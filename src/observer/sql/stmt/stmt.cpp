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

#include "sql/stmt/stmt.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/create_index_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/desc_table_stmt.h"
#include "sql/stmt/drop_table_stmt.h"
#include "sql/stmt/exit_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/help_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/load_data_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/set_variable_stmt.h"
#include "sql/stmt/show_index_stmt.h"
#include "sql/stmt/show_tables_stmt.h"
#include "sql/stmt/trx_begin_stmt.h"
#include "sql/stmt/trx_end_stmt.h"
#include "sql/stmt/update_stmt.h"

RC Stmt::create_stmt(Db *db, ParsedSqlNode &sql_node, Stmt *&stmt) {
  stmt = nullptr;

  switch (sql_node.flag) {
  case SCF_INSERT: {
    return InsertStmt::create(db, *sql_node.node.insertion, stmt);
  }
  case SCF_DELETE: {
    return DeleteStmt::create(db, *sql_node.node.deletion, stmt);
  }
  case SCF_SELECT: {
    std::set<Field> tmp;
    return SelectStmt::create(db, *sql_node.node.selection, stmt, nullptr, tmp);
  }
  case SCF_UPDATE: {
    return UpdateStmt::create(db, *sql_node.node.update, stmt);
  }

  case SCF_EXPLAIN: {
    return ExplainStmt::create(db, *sql_node.node.explain, stmt);
  }

  case SCF_CREATE_INDEX: {
    return CreateIndexStmt::create(db, *sql_node.node.create_index, stmt);
  }

  case SCF_CREATE_TABLE: {
    return CreateTableStmt::create(db, *sql_node.node.create_table, stmt);
  }

  case SCF_DROP_TABLE: {
    return DropTableStmt::create(db, *sql_node.node.drop_table, stmt);
  }

  case SCF_DESC_TABLE: {
    return DescTableStmt::create(db, *sql_node.node.desc_table, stmt);
  }

  case SCF_HELP: {
    return HelpStmt::create(stmt);
  }

  case SCF_SHOW_TABLES: {
    return ShowTablesStmt::create(db, stmt);
  }

  case SCF_SHOW_INDEX: {
    return ShowIndexStmt::create(db, 
        *sql_node.node.show_index,
        stmt);
  }

  case SCF_BEGIN: {
    return TrxBeginStmt::create(stmt);
  }

  case SCF_COMMIT:
  case SCF_ROLLBACK: {
    return TrxEndStmt::create(sql_node.flag, stmt);
  }

  case SCF_EXIT: {
    return ExitStmt::create(stmt);
  }

  case SCF_SET_VARIABLE: {
    return SetVariableStmt::create(*sql_node.node.set_variable, stmt);
  }

  case SCF_LOAD_DATA: {
    return LoadDataStmt::create(db, *sql_node.node.load_data, stmt);
  }

  case SCF_CALC: {
    return CalcStmt::create(*sql_node.node.calc, stmt);
  }

  default: {
    LOG_INFO("Command::type %d doesn't need to create statement.", sql_node.flag);
  } break;
  }
  return RC::UNIMPLENMENT;
}
