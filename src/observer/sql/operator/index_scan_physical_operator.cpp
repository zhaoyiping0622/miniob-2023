/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/07/08.
//

#include "sql/operator/index_scan_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"
#include <cstring>

RC IndexScanPhysicalOperator::make_data(const std::vector<Value> &values, std::vector<FieldMeta> &meta, Table *table,
                                        std::vector<char> &out) {
  std::vector<char> ret;
  int size = 0;
  for (auto &field : meta) {
    size += field.len();
  }
  ret.resize(size);
  char *beg = ret.data();
  for (int i = 0; i < values.size() && i < meta.size(); i++) {
    Value &value = const_cast<Value &>(values[i]);
    if (value.attr_type() == TEXTS) {
      int offset;
      RC rc = table->add_text(value.get_string().c_str(), offset);
      if (rc != RC::SUCCESS)
        return rc;
      value.set_int(offset);
    } else {
      Value::convert(value.attr_type(), meta[i].type(), value);
    }
    memcpy(beg, value.data(), meta[i].len());
    beg += meta[i].len();
  }
  out.swap(ret);
  return RC::SUCCESS;
}

IndexScanPhysicalOperator::IndexScanPhysicalOperator(Table *table, Index *index, bool readonly,
                                                     const std::vector<Value> &left_value, bool left_inclusive,
                                                     const std::vector<Value> &right_value, bool right_inclusive)
    : table_(table), index_(index), readonly_(readonly), left_inclusive_(left_inclusive),
      right_inclusive_(right_inclusive) {
  std::vector<FieldMeta> fields = index_->index_meta().fields();
  size_ = 0;
  for (auto &field : fields) {
    size_ += field.len();
  }
  auto &table_meta = table_->table_meta();
  left_value_.resize(size_);
  right_value_.resize(size_);
  RC rc = RC::SUCCESS;
  rc = make_data(left_value, fields, table, left_value_);
  if (rc != RC::SUCCESS) {
    LOG_WARN("fail to make data");
  }
  rc = make_data(right_value, fields, table, right_value_);
  if (rc != RC::SUCCESS) {
    LOG_WARN("fail to make data");
  }
}

RC IndexScanPhysicalOperator::open(Trx *trx) {
  if (nullptr == table_ || nullptr == index_) {
    return RC::INTERNAL;
  }

  // TODO(zhaoyiping): 这里要改
  IndexScanner *index_scanner =
      index_->create_scanner(left_value_.data(), size_, left_inclusive_, right_value_.data(), size_, right_inclusive_);
  if (nullptr == index_scanner) {
    LOG_WARN("failed to create index scanner");
    return RC::INTERNAL;
  }

  record_handler_ = table_->record_handler();
  if (nullptr == record_handler_) {
    LOG_WARN("invalid record handler");
    index_scanner->destroy();
    return RC::INTERNAL;
  }
  index_scanner_ = index_scanner;

  tuple_.set_schema(table_, table_->table_meta().field_metas());

  trx_ = trx;
  return RC::SUCCESS;
}

RC IndexScanPhysicalOperator::next(Tuple *env_tuple) {
  RID rid;
  RC rc = RC::SUCCESS;

  record_page_handler_.cleanup();

  bool filter_result = false;
  while (RC::SUCCESS == (rc = index_scanner_->next_entry(&rid))) {
    rc = record_handler_->get_record(record_page_handler_, &rid, readonly_, &current_record_);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    tuple_.set_record(&current_record_);
    rc = filter(tuple_, filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (!filter_result) {
      continue;
    }

    rc = trx_->visit_record(table_, current_record_, readonly_);
    if (rc == RC::RECORD_INVISIBLE) {
      continue;
    } else {
      return rc;
    }
  }

  return rc;
}

RC IndexScanPhysicalOperator::close() {
  index_scanner_->destroy();
  index_scanner_ = nullptr;
  return RC::SUCCESS;
}

Tuple *IndexScanPhysicalOperator::current_tuple() {
  tuple_.set_record(&current_record_);
  return &tuple_;
}

void IndexScanPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs) {
  predicates_ = std::move(exprs);
}

RC IndexScanPhysicalOperator::filter(RowTuple &tuple, bool &result) {
  RC rc = RC::SUCCESS;
  Value value;
  for (std::unique_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}

std::string IndexScanPhysicalOperator::param() const {
  return std::string(index_->index_meta().name()) + " ON " + table_->name();
}
