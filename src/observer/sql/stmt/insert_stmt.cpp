/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
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

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <utility>

InsertStmt::InsertStmt(Table *table, std::vector<std::vector<Value>> records) : table_(table), records_(records) {}

RC InsertStmt::create(Db *db, const InsertSqlNode &inserts, Stmt *&stmt) {
  const char *table_name = inserts.relation_name.c_str();
  if (nullptr == db || nullptr == table_name || inserts.values.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d", db, table_name,
             static_cast<int>(inserts.values.size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  auto &records = inserts.values;
  auto &names = inserts.name_lists;
  if (names.size()) {
    if (names.size() != records[0].size()) {
      return RC::INVALID_ARGUMENT;
    }
  }
  vector<vector<Value>> record_values;
  RC rc;

  for (auto &record_expr : records) {
    vector<Value> record;
    record.reserve(record_expr.size());
    for (int i = 0; i < record_expr.size(); i++) {
      Value value;
      Expression *expr;
      rc = Expression::create(nullptr, nullptr, nullptr, record_expr[i], expr, nullptr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to create expression in insert value");
        return rc;
      }
      rc = expr->try_get_value(value);
      delete expr;
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to get value in insert value");
        return rc;
      }
      record.push_back(value);
    }
    rc = check_record(db, table, record, names);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    record_values.push_back(record);
  }

  if (table->view()) {
    auto *view = table->view();
    std::vector<int> tmp;
    table = view->view_meta().get_insert_table(db, tmp);
    if (table == nullptr)
      return RC::INVALID_ARGUMENT;
  }

  // everything alright
  stmt = new InsertStmt(table, record_values);
  return RC::SUCCESS;
}

RC InsertStmt::check_record(Db *db, Table *table, std::vector<Value> &record, const std::vector<std::string> &names) {
  auto *view = table->view();

  if (names.size()) {
    std::vector<Value> tmp;
    const TableMeta &table_meta = table->table_meta();
    int null_cnt = 0;
    for (int i = table_meta.sys_field_num(); i < table_meta.field_num(); i++) {
      bool found = false;
      for (int j = 0; j < names.size(); j++) {
        if (table_meta.field(i)->name() == names[j]) {
          found = true;
          tmp.push_back(std::move(record[j]));
        }
      }
      if (!found) {
        LOG_WARN("field %s not found in column list", table_meta.field(i)->name());
        return RC::INVALID_ARGUMENT;
      }
    }
    record = tmp;
  }

  if (view != nullptr) {
    std::vector<int> order;
    table = view->view_meta().get_insert_table(db, order);
    if (table == nullptr) {
      LOG_WARN("view %s is not insertable", view->view_meta().name().c_str());
      return RC::INVALID_ARGUMENT;
    }
    std::vector<Value> values;
    for (auto x : order) {
      if (x != -1)
        values.push_back(std::move(record[x]));
      else {
        Value tmp;
        tmp.set_null();
        values.push_back(tmp);
      }
    }
    record.swap(values);
  }

  const TableMeta &table_meta = table->table_meta();
  const char *table_name = table_meta.name();
  const int field_num = table_meta.field_num() - table_meta.sys_field_num();
  const int value_num = static_cast<int>(record.size());

  // check the fields number
  if (field_num != value_num) {
    LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
    return RC::SCHEMA_FIELD_MISSING;
  }

  // check fields type
  const int sys_field_num = table_meta.sys_field_num();
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
    const AttrType field_type = field_meta->type();
    const AttrType value_type = record[i].attr_type();
    if (value_type == NULLS) {
      if (!field_meta->nullable()) {
        LOG_WARN("field can not be null");
        return RC::INVALID_ARGUMENT;
      }
    } else if (!Value::convert(value_type, field_type,
                               const_cast<Value &>(record[i]))) { // TODO try to convert the value type to field type
      LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d", table_name, field_meta->name(),
               field_type, value_type);
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }
  return RC::SUCCESS;
}
