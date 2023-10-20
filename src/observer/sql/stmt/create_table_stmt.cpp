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
// Created by Wangyunlai on 2023/6/13.
//

#include "sql/stmt/create_table_stmt.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "event/sql_debug.h"

RC CreateTableStmt::create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt) {
  auto *create_table_stmt = new CreateTableStmt(create_table.relation_name, create_table.attr_infos);
  stmt = create_table_stmt;
  create_table_stmt->db_ = db;
  RC rc = RC::SUCCESS;
  if (create_table.select != nullptr) {
    Stmt *select_stmt;
    std::set<Field> fields;
    rc = Stmt::create_stmt(db, *create_table.select, select_stmt);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (fields.size()) {
      LOG_WARN("select stmt use father fields");
      return RC::INVALID_ARGUMENT;
    }
    create_table_stmt->select_stmt_.reset(static_cast<SelectStmt *>(select_stmt));
  }
  if (create_table.attr_infos.empty() && create_table.select == nullptr) {
    LOG_WARN("table %s has no fields", create_table.relation_name.c_str());
    return RC::INVALID_ARGUMENT;
  }
  // sql_debug("create table statement: table name %s", create_table.relation_name.c_str());
  return RC::SUCCESS;
}
