/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Wangyunlai on 2021/5/13.
//

#include <algorithm>
#include <cassert>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include "common/defs.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/value.h"
#include "storage/buffer/disk_buffer_pool.h"
#include "storage/buffer/page.h"
#include "storage/common/meta_util.h"
#include "storage/field/field.h"
#include "storage/index/bplus_tree_index.h"
#include "storage/index/index.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"
#include "storage/table/table_meta.h"
#include "storage/trx/trx.h"
#include "storage/view/view.h"

Table::Table(View *view) : view_(view) {}

Table::~Table() {
  if (record_handler_ != nullptr) {
    delete record_handler_;
    record_handler_ = nullptr;
  }

  if (data_buffer_pool_ != nullptr) {
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
  }

  if (text_buffer_pool_ != nullptr) {
    text_buffer_pool_->close_file();
    text_buffer_pool_ = nullptr;
  }

  for (std::vector<Index *>::iterator it = indexes_.begin(); it != indexes_.end(); ++it) {
    Index *index = *it;
    delete index;
  }
  indexes_.clear();

  LOG_INFO("Table has been closed: %s", name());
}

RC Table::create(int32_t table_id, const char *path, const char *name, const char *base_dir, int attribute_count,
                 const AttrInfoSqlNode attributes[]) {

  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create table %s:%s", base_dir, name);

  if (attribute_count <= 0 || nullptr == attributes) {
    LOG_WARN("Invalid arguments. table_name=%s, attribute_count=%d, attributes=%p", name, attribute_count, attributes);
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;

  // 使用 table_name.table记录一个表的元数据
  // 判断表文件是否已经存在
  int fd = ::open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
  if (fd < 0) {
    if (EEXIST == errno) {
      LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s", path, strerror(errno));
      return RC::SCHEMA_TABLE_EXIST;
    }
    LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s", path, errno, strerror(errno));
    return RC::IOERR_OPEN;
  }

  close(fd);

  // 创建文件
  if ((rc = table_meta().init(table_id, name, attribute_count, attributes)) != RC::SUCCESS) {
    LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
    return rc; // delete table file
  }

  std::fstream fs;
  fs.open(path, std::ios_base::out | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", path, strerror(errno));
    return RC::IOERR_OPEN;
  }

  // 记录元数据到文件中
  table_meta().serialize(fs);
  fs.close();

  std::string data_file = table_data_file(base_dir, name);
  BufferPoolManager &bpm = BufferPoolManager::instance();
  rc = bpm.create_file(data_file.c_str());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create disk buffer pool of data file. file name=%s", data_file.c_str());
    return rc;
  }

  rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s due to init record handler failed.", data_file.c_str());
    // don't need to remove the data_file
    return rc;
  }

  auto text_file = table_text_file(base_dir, name);
  rc = bpm.create_file(text_file.c_str());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create disk buffer pool of text file. file name=%s", text_file.c_str());
    return rc;
  }

  rc = init_text_buffer_pool(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s due to init text buffer pool failed.", name);
    return rc;
  }

  base_dir_ = base_dir;
  LOG_INFO("Successfully create table %s:%s", base_dir, name);
  return rc;
}

RC Table::open(const char *meta_file, const char *base_dir) {
  // 加载元数据文件
  std::fstream fs;
  std::string meta_file_path = std::string(base_dir) + common::FILE_PATH_SPLIT_STR + meta_file;
  fs.open(meta_file_path, std::ios_base::in | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file_path.c_str(), strerror(errno));
    return RC::IOERR_OPEN;
  }
  if (table_meta().deserialize(fs) < 0) {
    LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file_path.c_str());
    fs.close();
    return RC::INTERNAL;
  }
  fs.close();

  // 加载数据文件
  RC rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open table %s due to init record handler failed.", base_dir);
    // don't need to remove the data_file
    return rc;
  }

  rc = init_text_buffer_pool(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s due to init text buffer pool failed.", base_dir);
    return rc;
  }

  base_dir_ = base_dir;

  const int index_num = table_meta().index_num();
  for (int i = 0; i < index_num; i++) {
    const IndexMeta *index_meta = table_meta().index(i);

    BplusTreeIndex *index = new BplusTreeIndex();
    std::string index_file = table_index_file(base_dir, name(), index_meta->name());
    rc = index->open(index_file.c_str(), this, *index_meta);
    if (rc != RC::SUCCESS) {
      delete index;
      LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%s", name(), index_meta->name(),
                index_file.c_str(), strrc(rc));
      // skip cleanup
      //  do all cleanup action in destructive Table function.
      return rc;
    }
    indexes_.push_back(index);
  }

  return rc;
}

RC Table::insert_record(Record &record) {
  RC rc = RC::SUCCESS;
  rc = record_handler_->insert_record(record.data(), table_meta().record_size(), &record.rid());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Insert record failed. table name=%s, rc=%s", table_meta().name(), strrc(rc));
    return rc;
  }

  rc = insert_entry_of_indexes(record.data(), record.rid());
  if (rc != RC::SUCCESS) { // 可能出现了键值重复
    RC rc2 = delete_entry_of_indexes(record.data(), record.rid(), false /*error_on_not_exists*/);
    if (rc2 != RC::SUCCESS) {
      LOG_ERROR("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s", name(), rc2,
                strrc(rc2));
    }
    rc2 = record_handler_->delete_record(&record.rid());
    if (rc2 != RC::SUCCESS) {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s", name(), rc2,
                strrc(rc2));
    }
  }
  return rc;
}

RC Table::visit_record(const RID &rid, bool readonly, std::function<void(Record &)> visitor) {
  return record_handler_->visit_record(rid, readonly, visitor);
}

RC Table::get_record(const RID &rid, Record &record) {
  const int record_size = table_meta().record_size();
  char *record_data = (char *)malloc(record_size);
  ASSERT(nullptr != record_data, "failed to malloc memory. record data size=%d", record_size);

  auto copier = [&record, record_data, record_size](Record &record_src) {
    memcpy(record_data, record_src.data(), record_size);
    record.set_rid(record_src.rid());
  };
  RC rc = record_handler_->visit_record(rid, true /*readonly*/, copier);
  if (rc != RC::SUCCESS) {
    free(record_data);
    LOG_WARN("failed to visit record. rid=%s, table=%s, rc=%s", rid.to_string().c_str(), name(), strrc(rc));
    return rc;
  }

  record.set_data_owner(record_data, record_size);
  return rc;
}

RC Table::recover_insert_record(Record &record) {
  RC rc = RC::SUCCESS;
  rc = record_handler_->recover_insert_record(record.data(), table_meta().record_size(), record.rid());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Insert record failed. table name=%s, rc=%s", table_meta().name(), strrc(rc));
    return rc;
  }

  rc = insert_entry_of_indexes(record.data(), record.rid());
  if (rc != RC::SUCCESS) { // 可能出现了键值重复
    RC rc2 = delete_entry_of_indexes(record.data(), record.rid(), false /*error_on_not_exists*/);
    if (rc2 != RC::SUCCESS) {
      LOG_ERROR("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s", name(), rc2,
                strrc(rc2));
    }
    rc2 = record_handler_->delete_record(&record.rid());
    if (rc2 != RC::SUCCESS) {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s", name(), rc2,
                strrc(rc2));
    }
  }
  return rc;
}

const char *Table::name() const { return table_meta().name(); }

const TableMeta &Table::table_meta() const { return const_cast<Table *>(this)->table_meta(); }

TableMeta &Table::table_meta() {
  if (view()) {
    return view()->table_meta();
  }
  return table_meta_;
}

RC Table::make_record(char *data, int len, Record &record) {
  record.set_data(data, len);
  return RC::SUCCESS;
}

RC Table::make_record(int value_num, const Value *values, Record &record) {
  // 检查字段类型是否一致
  if (value_num + table_meta().sys_field_num() != table_meta().field_num()) {
    LOG_WARN("Input values don't match the table's schema, table name:%s", table_meta().name());
    return RC::SCHEMA_FIELD_MISSING;
  }

  const int normal_field_start_index = table_meta().sys_field_num();
  RC rc = RC::SUCCESS;
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field = table_meta().field(i + normal_field_start_index);
    Value &value = const_cast<Value &>(values[i]);
    if (field->type() == TEXTS) {
      if (value.get_string().size()) {
        int offset;
        rc = add_text(value.get_string().c_str(), offset);
        if (rc != RC::SUCCESS)
          return rc;
        value.set_int(offset);
      }
    } else if (value.attr_type() == NULLS) {
      if (!field->nullable()) {
        LOG_ERROR("unreachable should be checked in caller");
        return RC::INVALID_ARGUMENT;
      }
    } else if (field->type() != value.attr_type()) {
      if (!Value::convert(value.attr_type(), field->type(), value)) {
        LOG_ERROR("Invalid value type. table name =%s, field name=%s, type=%d, but given=%d", table_meta().name(),
                  field->name(), field->type(), value.attr_type());
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    }
    // 对于超过长度的字符串报错
    if (field->type() == CHARS) {
      if (value.length() > field->len()) {
        return RC::INVALID_ARGUMENT;
      }
    }
  }

  // 复制所有字段的值
  int record_size = table_meta().record_size();
  char *record_data = (char *)malloc(record_size);
  memset(record_data, 0, record_size);

  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field = table_meta().field(i + normal_field_start_index);
    const Value &value = values[i];
    size_t copy_len = field->len();
    if (value.attr_type() == NULLS) {
      int null_offset = table_meta().null_field_meta()->offset();
      int &null_value = *(int *)(record_data + null_offset);
      null_value |= 1 << (field->index());
    } else if (field->type() == CHARS) {
      copy_len = std::min(value.get_string().length(), copy_len);
      memcpy(record_data + field->offset(), value.get_string().c_str(), copy_len);
    } else {
      memcpy(record_data + field->offset(), value.data(), copy_len);
    }
  }

  record.set_data_owner(record_data, record_size);
  return RC::SUCCESS;
}

RC Table::init_record_handler(const char *base_dir) {
  std::string data_file = table_data_file(base_dir, table_meta().name());

  RC rc = BufferPoolManager::instance().open_file(data_file.c_str(), data_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", data_file.c_str(), rc, strrc(rc));
    return rc;
  }

  record_handler_ = new RecordFileHandler();
  rc = record_handler_->init(data_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to init record handler. rc=%s", strrc(rc));
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
    delete record_handler_;
    record_handler_ = nullptr;
    return rc;
  }

  return rc;
}

RC Table::get_record_scanner(RecordFileScanner &scanner, Trx *trx, bool readonly) {
  RC rc = scanner.open_scan(this, *data_buffer_pool_, trx, readonly);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to open scanner. rc=%s", strrc(rc));
  }
  return rc;
}

RC Table::create_index(Trx *trx, const std::vector<FieldMeta> &field_meta, const char *index_name, bool unique) {
  if (common::is_blank(index_name) || field_meta.empty()) {
    LOG_INFO("Invalid input arguments, table name is %s, index_name is blank or attribute_name is blank", name());
    return RC::INVALID_ARGUMENT;
  }

  auto real_meta = field_meta;
  real_meta.insert(real_meta.begin(), *table_meta().null_field_meta());

  IndexMeta new_index_meta;
  RC rc = new_index_meta.init(index_name, real_meta, unique);
  if (rc != RC::SUCCESS) {
    LOG_INFO("Failed to init IndexMeta in table:%s, index_name:%s", name(), index_name);
    return rc;
  }

  // 创建索引相关数据
  BplusTreeIndex *index = new BplusTreeIndex();
  std::string index_file = table_index_file(base_dir_.c_str(), name(), index_name);
  rc = index->create(index_file.c_str(), this, new_index_meta);
  if (rc != RC::SUCCESS) {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
    return rc;
  }

  // 遍历当前的所有数据，插入这个索引
  RecordFileScanner scanner;
  rc = get_record_scanner(scanner, trx, true /*readonly*/);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create scanner while creating index. table=%s, index=%s, rc=%s", name(), index_name, strrc(rc));
    return rc;
  }

  Record record;
  while (scanner.has_next()) {
    rc = scanner.next(record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to scan records while creating index. table=%s, index=%s, rc=%s", name(), index_name, strrc(rc));
      return rc;
    }
    rc = index->insert_entry(record.data(), &record.rid());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record into index while creating index. table=%s, index=%s, rc=%s", name(), index_name,
               strrc(rc));
      return rc;
    }
  }
  scanner.close_scan();
  LOG_INFO("inserted all records into new index. table=%s, index=%s", name(), index_name);

  indexes_.push_back(index);

  /// 接下来将这个索引放到表的元数据中
  TableMeta new_table_meta(table_meta());
  rc = new_table_meta.add_index(new_index_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }
  rc = flush_table_meta_file(new_table_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to edit meta file while adding index (%s) on table (%s). error=%d:%s", index_name, name(), rc,
              strrc(rc));
    return rc;
  }
  return rc;
}

RC Table::drop_index(int idx) {
  assert(idx < indexes_.size());
  string index_name = indexes_[idx]->index_meta().name();
  // 这里的析构函数会把所有buffer里相关的page都刷下去
  delete indexes_[idx];
  indexes_.erase(indexes_.begin() + idx);
  auto index_file_name = table_index_file(base_dir_.c_str(), name(), index_name.c_str());
  if (unlink(index_file_name.c_str()) < 0) {
    LOG_ERROR("failed to remove index file %s while droping index (%s) on table (%s). error=%d:%s",
              index_file_name.c_str(), index_name.c_str(), name(), errno, strerror(errno));
    return RC::IOERR_UNLINK;
  }
  return RC::SUCCESS;
}

RC Table::drop_index(const char *index_name) {
  TableMeta new_table_meta(table_meta());
  RC rc = new_table_meta.drop_index(index_name);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to drop index (%s) on table(%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }
  rc = flush_table_meta_file(new_table_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to edit meta file while droping index (%s) on table (%s). error=%d:%s", index_name, name(), rc,
              strrc(rc));
    return rc;
  }
  for (int i = 0; i < indexes_.size(); i++) {
    if (strcmp(indexes_[i]->index_meta().name(), index_name) == 0) {
      return drop_index(i);
    }
  }
  return RC::SCHEMA_INDEX_NOT_EXIST;
}

RC Table::drop_all_indexes() {
  TableMeta new_table_meta(table_meta());
  RC rc = RC::SUCCESS;
  for (int i = static_cast<int>(indexes_.size()) - 1; i >= 0; i--) {
    rc = new_table_meta.drop_index(indexes_[i]->index_meta().name());
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to drop index (%s) on table(%s). error=%d:%s", indexes_[i]->index_meta().name(), name(), rc,
                strrc(rc));
      return rc;
    }
  }
  rc = flush_table_meta_file(new_table_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to edit meta file while droping all index on table (%s). error=%d:%s", name(), rc, strrc(rc));
    return rc;
  }
  for (int i = static_cast<int>(indexes_.size()) - 1; i >= 0; i--) {
    rc = drop_index(i);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to drop index %d while droping all index on table (%s). error=%d:%s", i, name(), rc, strrc(rc));
      return rc;
    }
  }
  return RC::SUCCESS;
}

RC Table::delete_record(const RID &rid) {
  RC rc = RC::SUCCESS;
  RC rc1 = RC::SUCCESS;
  rc1 = visit_record(rid, false, [&](Record &record) { rc = delete_record(record); });
  if (rc1 != RC::SUCCESS)
    return rc1;
  return rc;
}

RC Table::delete_record(const Record &record) {
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->delete_entry(record.data(), &record.rid());
    ASSERT(RC::SUCCESS == rc, "failed to delete entry from index. table name=%s, index name=%s, rid=%s, rc=%s", name(),
           index->index_meta().name(), record.rid().to_string().c_str(), strrc(rc));
  }
  rc = record_handler_->delete_record(&record.rid());
  return rc;
}

RC Table::insert_entry_of_indexes(const char *record, const RID &rid) {
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->insert_entry(record, &rid);
    if (rc != RC::SUCCESS) {
      break;
    }
  }
  return rc;
}

RC Table::delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists) {
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->delete_entry(record, &rid);
    if (rc != RC::SUCCESS) {
      if (rc == RC::RECORD_NOT_EXIST) {
        if (error_on_not_exists)
          return rc;
        return RC::SUCCESS;
      }
      if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists) {
        break;
      }
    }
  }
  return rc;
}

Index *Table::find_index(const char *index_name) const {
  for (Index *index : indexes_) {
    if (0 == strcmp(index->index_meta().name(), index_name)) {
      return index;
    }
  }
  return nullptr;
}
Index *Table::find_index_by_field(const char *field_name) const {
  const TableMeta &table_meta = this->table_meta();
  const IndexMeta *index_meta = table_meta.find_index_by_field(field_name);
  if (index_meta != nullptr) {
    return this->find_index(index_meta->name());
  }
  return nullptr;
}

Index *Table::find_index_by_fields(std::vector<const char *> fields) const {
  const TableMeta &table_meta = this->table_meta();
  const IndexMeta *index_meta = table_meta.find_index_by_fields(fields);
  if (index_meta != nullptr) {
    return this->find_index(index_meta->name());
  }
  return nullptr;
}

RC Table::sync() {
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->sync();
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to flush index's pages. table=%s, index=%s, rc=%d:%s", name(), index->index_meta().name(), rc,
                strrc(rc));
      return rc;
    }
  }
  LOG_INFO("Sync table over. table=%s", name());
  return rc;
}

RC Table::flush_table_meta_file(TableMeta &new_table_meta) {
  /// 内存中有一份元数据，磁盘文件也有一份元数据。修改磁盘文件时，先创建一个临时文件，写入完成后再rename为正式文件
  /// 这样可以防止文件内容不完整
  // 创建元数据临时文件
  std::string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  std::fstream fs;
  fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR_OPEN; // 创建索引中途出错，要做还原操作
  }
  if (new_table_meta.serialize(fs) < 0) {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }
  fs.close();

  // 覆盖原始元数据文件
  std::string meta_file = table_meta_file(base_dir_.c_str(), name());
  int ret = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0) {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while editing meta file on table (%s). "
              "system error=%d:%s",
              tmp_file.c_str(), meta_file.c_str(), name(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }

  table_meta().swap(new_table_meta);

  return RC::SUCCESS;
}

RC Table::init_text_buffer_pool(const char *base_dir) {
  std::string text_file = table_text_file(base_dir, table_meta().name());

  RC rc = BufferPoolManager::instance().open_file(text_file.c_str(), text_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", text_file.c_str(), rc, strrc(rc));
    return rc;
  }

  return rc;
}

// FIXME(zhaoyiping): 这里有个溢出

const int TEXT_PAGE_SIZE = BP_PAGE_SIZE;
const int TEXT_PAGE_MAX_NUM = TEXT_SIZE / TEXT_PAGE_SIZE + !!(TEXT_SIZE % TEXT_PAGE_SIZE);

RC Table::get_text(int offset, Value &value) {
  std::string tmp;
  int page_num = offset / BP_PAGE_SIZE;
  for (int i = 0; i < TEXT_SIZE / BP_PAGE_SIZE; i++) {
    Frame *frame = nullptr;
    RC rc = text_buffer_pool_->get_this_page(page_num + i, &frame);
    if (rc != RC::SUCCESS)
      return rc;
    bool end = false;
    for (int j = 0; j < BP_PAGE_SIZE && !end; j++) {
      if (frame->data()[j] == 0) {
        end = true;
      } else {
        tmp += frame->data()[j];
      }
    }
    frame->unpin();
    if (end)
      break;
  }
  value.set_text(tmp.c_str());
  return RC::SUCCESS;
}

RC Table::add_text(const char *data, int &offset) {
  int len = strlen(data) + 1;
  if (len > TEXT_SIZE) {
    return RC::INVALID_ARGUMENT;
  }
  int num = (len - 1) / BP_PAGE_SIZE + 1;

  int ret = -1;

  RC rc = RC::SUCCESS;

  for (int i = 0; i < num; i++) {
    Frame *frame = nullptr;
    rc = text_buffer_pool_->allocate_page(&frame);
    if (rc != RC::SUCCESS)
      return rc;
    if (ret == -1) {
      ret = frame->page_num() * BP_PAGE_SIZE;
    }
    frame->clear_page();
    strncpy(frame->data(), data + i * BP_PAGE_SIZE, BP_PAGE_SIZE);
    frame->mark_dirty();
    frame->unpin();
  }
  offset = ret;
  return RC::SUCCESS;
}
