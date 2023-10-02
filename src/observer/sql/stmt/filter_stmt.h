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

#pragma once

#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include <memory>
#include <unordered_map>
#include <vector>

class Db;
class Table;
class FieldMeta;

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt {
public:
  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  std::unique_ptr<Expression> &filter_expr() { return filter_expr_; }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ConjunctionExprSqlNode *conditions, FilterStmt *&stmt, ExprGenerator *generator);

private:
  std::unique_ptr<Expression> filter_expr_;
};
