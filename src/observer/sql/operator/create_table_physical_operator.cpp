#include "sql/operator/create_table_physical_operator.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include <utility>

RC CreateTablePhysicalOperator::open(Trx *trx) {
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return db_->create_table(table_name_.c_str(), attr_infos_.size(), attr_infos_.data());
  }
  if (children_.size() > 1) {
    LOG_WARN("create table has more than one children");
    return RC::INTERNAL;
  }
  std::vector<std::vector<Value>> value_list;
  auto &child = children_[0];
  rc = child->open(trx);
  if (rc != RC::SUCCESS)
    return rc;
  while ((rc = child->next(nullptr)) == RC::SUCCESS) {
    std::vector<Value> values;
    auto *tuple = child->current_tuple();
    if (schema_->cell_num() != tuple->cell_num()) {
      LOG_WARN("schema cell_num not equals to tuple cell_num");
      return RC::INTERNAL;
    }
    for (int i = 0; i < schema_->cell_num(); i++) {
      auto &spec = schema_->cell_at(i);
      Value value;
      rc = tuple->cell_at(i, value);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to get cell");
        return rc;
      }
      values.push_back(value);
    }
    value_list.emplace_back(std::move(values));
  }
  if (rc != RC::RECORD_EOF)
    return rc;
  child->close();
  std::set<std::string> field_names;
  for (int i = 0; i < schema_->cell_num(); i++) {
    auto &spec = schema_->cell_at(i);
    std::string name;
    if (common::is_blank(spec.table_name())) {
      name = spec.alias();
    } else {
      name = spec.field_name();
    }
    if (field_names.count(name))
      return RC::INVALID_ARGUMENT;
    field_names.insert(name);
    if (i < attr_infos_.size()) {
      if (attr_infos_[i].name != name || attr_infos_[i].type != types_[i].type ||
          types_[i].length && attr_infos_[i].length != types_[i].length ||
          types_[i].nullable != attr_infos_[i].nullable) {
        return RC::INVALID_ARGUMENT;
      }
    } else {
      if (types_[i].type == CHARS && types_[i].length == 0)
        types_[i].type = TEXTS;
      attr_infos_.push_back(AttrInfoSqlNode{.type = types_[i].type,
                                            .name = name,
                                            .length = static_cast<size_t>(types_[i].length),
                                            .nullable = types_[i].nullable});
    }
    if (attr_infos_[i].length == 0) {
      attr_infos_[i].length = attr_type_to_size(attr_infos_[i].type);
    }
  }
  rc = db_->create_table(table_name_.c_str(), attr_infos_.size(), attr_infos_.data());
  if (rc != RC::SUCCESS)
    return rc;
  Table *table = db_->find_table(table_name_.c_str());
  if (table == nullptr) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  for (auto &values : value_list) {
    Record record;
    rc = table->make_record(values.size(), values.data(), record);
    if (rc != RC::SUCCESS) {
      goto fail;
    }
    rc = table->insert_record(record);
    if (rc != RC::SUCCESS) {
      goto fail;
    }
  }
  return RC::SUCCESS;
fail:
  db_->drop_table(trx, table_name_.c_str());
  return rc;
}
