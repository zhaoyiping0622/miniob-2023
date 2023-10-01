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

#include "sql/stmt/filter_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

FilterStmt::~FilterStmt() {}

RC FilterStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                      const ConjunctionExprSqlNode *conditions, FilterStmt *&stmt, ExprGenerator *generator) {
  Expression *expr;
  RC rc = RC::SUCCESS;
  stmt = nullptr;
  rc = ConjunctionExpr::create(db, default_table, tables, conditions, expr, generator);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create ConjunctionExpr");
    return rc;
  }
  stmt = new FilterStmt();
  stmt->filter_expr_.reset(expr);
  return rc;
}
