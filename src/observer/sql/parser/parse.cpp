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
// Created by Meiyi
//

#include "sql/parser/parse.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include <mutex>

RC parse(char *st, ParsedSqlNode *sqln);

CalcSqlNode::~CalcSqlNode() {
  for (auto *expr : expressions) {
    delete expr;
  }
  expressions.clear();
}

ParsedSqlNode::ParsedSqlNode() : flag(SCF_ERROR) {}

ParsedSqlNode::ParsedSqlNode(SqlCommandFlag _flag) : flag(_flag) {}

void ParsedSqlResult::add_sql_node(std::unique_ptr<ParsedSqlNode> sql_node) {
  sql_nodes_.emplace_back(std::move(sql_node));
}

ConjunctionExprSqlNode::~ConjunctionExprSqlNode() {
  if (left)
    delete left;
  if (right)
    delete right;
}

ComparisonExprSqlNode::~ComparisonExprSqlNode() {
  if (left)
    delete left;
  if (right)
    delete right;
}

ArithmeticExprSqlNode::~ArithmeticExprSqlNode() {
  if (left)
    delete left;
  if (right)
    delete right;
}

AggregationExprSqlNode::~AggregationExprSqlNode() {
  if (child)
    delete child;
}

ExprSqlNode::~ExprSqlNode() {
  switch (type_) {
  case ExprType::STAR: delete expr_.star; break;
  case ExprType::FIELD: delete expr_.field; break;
  case ExprType::VALUE: delete expr_.value; break;
  case ExprType::COMPARISON: delete expr_.comparison; break;
  case ExprType::CONJUNCTION: delete expr_.conjunction; break;
  case ExprType::ARITHMETIC: delete expr_.arithmetic; break;
  case ExprType::AGGREGATION: delete expr_.aggregation; break;
  default: break;
  }
}

////////////////////////////////////////////////////////////////////////////////

int sql_parse(const char *st, ParsedSqlResult *sql_result);

RC parse(const char *st, ParsedSqlResult *sql_result) {
  sql_parse(st, sql_result);
  return RC::SUCCESS;
}
