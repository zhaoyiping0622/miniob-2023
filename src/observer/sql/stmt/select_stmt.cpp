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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <memory>

SelectStmt::~SelectStmt() {
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

static void wildcard_fields(Table *table, std::vector<std::unique_ptr<Expression>> &field_metas) {
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.emplace_back(new FieldExpr(table, table_meta.field(i)));
  }
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt) {
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].c_str();
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables.push_back(table);
    table_map.insert(std::pair<std::string, Table *>(table_name, table));
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  unique_ptr<TupleSchema> schema = make_unique<TupleSchema>();

  // collect query fields in `select` statement
  std::vector<std::unique_ptr<Expression>> expressions;
  for (int i = 0; i < select_sql.attributes.size(); i++) {
    const ExprSqlNode *expression = select_sql.attributes[i];

    if (expression->type() == ExprType::STAR) {
      for (Table *table : tables) {
        wildcard_fields(table, expressions);
      }
    } else {
      Expression *expr;
      RC rc = Expression::create(db, default_table, &table_map, expression, expr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to parse expression, rc=%s", strrc(rc));
        return rc;
      }
      expressions.emplace_back(expr);
    }
  }

  vector<set<Field>> reference_fields(expressions.size());
  set<Field> used_fields;

  for (int i = 0; i < expressions.size(); i++) {
    auto fields = expressions[i]->reference_fields();
    used_fields.insert(fields.begin(), fields.end());
    if (expressions[i]->type() == ExprType::FIELD) {
      Field field = *fields.begin();
      schema->append_cell(field.table_name(), field.field_name());
    } else {
      schema->append_cell(expressions[i]->name().c_str());
    }
    reference_fields[i].swap(fields);
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), expressions.size());

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  if (select_sql.conditions != nullptr) {
    RC rc = FilterStmt::create(db, default_table, &table_map, select_sql.conditions, filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->tables_.swap(tables);
  select_stmt->used_fields_ = used_fields;
  select_stmt->reference_fields_.swap(reference_fields);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->expressions_.swap(expressions);
  select_stmt->schema_ = std::move(schema);
  stmt = select_stmt;
  return RC::SUCCESS;
}
