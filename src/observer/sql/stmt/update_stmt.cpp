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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/value.h"

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt) {
  Table *table = db->find_table(update.relation_name.c_str());
  if (table == nullptr) {
    LOG_WARN("table %s not exists", update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  const FieldMeta *field_meta;
  RC rc = RC::SUCCESS;
  field_meta = table->table_meta().field(update.attribute_name.c_str());
  if (field_meta == nullptr) {
    LOG_WARN("field %s not exist in table %s", update.attribute_name.c_str(), update.relation_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  Expression *value_expr;
  std::unordered_map<std::string, Table *> table_map;
  table_map.emplace(table->name(), table);
  rc = ValueExpr::create(&update.value, value_expr);
  if (rc != RC::SUCCESS)
    return rc;
  Value value;
  rc = value_expr->try_get_value(value);
  if (rc != RC::SUCCESS)
    return rc;
  FilterStmt *filter = nullptr;
  if (update.conditions) {
    rc = FilterStmt::create(db, table, &table_map, update.conditions, filter, nullptr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("fail to create filter stmt");
      return rc;
    }
  }
  if (field_meta->type() != value.attr_type()) {
    if (field_meta->type() == TEXTS) {
      int page_of;
      rc = table->add_text(value.get_string().c_str(), page_of);
      if (rc != RC::SUCCESS)
        return rc;
      value.set_int(page_of);
    } else {
      if (!Value::convert(value.attr_type(), field_meta->type(), value)) {
        LOG_WARN("update value cannot convert into target type, src=%s, target=%s",
                 attr_type_to_string(value.attr_type()), attr_type_to_string(field_meta->type()));
        return RC::INVALID_ARGUMENT;
      }
    }
  }
  if (value.attr_type() == CHARS)
    value.get_fiexed_string();
  auto *update_stmt = new UpdateStmt();
  update_stmt->table_ = table;
  update_stmt->field_ = Field(table, field_meta);
  update_stmt->value_ = value;
  update_stmt->filter_ = filter;
  stmt = update_stmt;
  return RC::SUCCESS;
}
