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

ParsedSqlNode::ParsedSqlNode() : flag(SCF_ERROR) { node.error = nullptr; }

ParsedSqlNode::ParsedSqlNode(SqlCommandFlag _flag) : flag(_flag) { node.error = nullptr; }

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
  for (auto child : children)
    delete child;
}

NamedExprSqlNode::~NamedExprSqlNode() {
  switch (type) {
  case NamedType::AGGREGATION: delete child.aggr; break;
  case NamedType::SUBQUERY: {
  }
  }
}

ExprSqlNode::~ExprSqlNode() {
  switch (type_) {
  case ExprType::STAR: delete expr_.star; break;
  case ExprType::FIELD: delete expr_.field; break;
  case ExprType::VALUE: delete expr_.value; break;
  case ExprType::COMPARISON: delete expr_.comparison; break;
  case ExprType::CONJUNCTION: delete expr_.conjunction; break;
  case ExprType::ARITHMETIC: delete expr_.arithmetic; break;
  case ExprType::NAMED: delete expr_.named; break;
  case ExprType::FUNCTION: delete expr_.function; break;
  case ExprType::CONTAIN: delete expr_.contain; break;
  case ExprType::LIKE: delete expr_.like; break;
  case ExprType::LIST: delete expr_.list; break;
  case ExprType::SET: delete expr_.set; break;
  case ExprType::NULL_CHECK: delete expr_.null; break;
  default: break;
  }
}

FunctionExprSqlNode::~FunctionExprSqlNode() {
  for (auto x : children)
    delete x;
}

ContainExprSqlNode::~ContainExprSqlNode() {
  if (left)
    delete left;
  if (right)
    delete right;
}

ExistsExprSqlNode::~ExistsExprSqlNode() {
  if (left)
    delete left;
}

LikeExprSqlNode::~LikeExprSqlNode() {
  if (left)
    delete left;
}

NullCheckExprSqlNode::~NullCheckExprSqlNode() {
  if (left)
    delete left;
}

ListExprSqlNode::~ListExprSqlNode() {
  if (select)
    delete select;
}

ParsedSqlNode::~ParsedSqlNode() {
  void *tmp = reinterpret_cast<void *>(node.error);
  if (tmp == nullptr)
    return;
  switch (flag) {
  case SCF_ERROR: delete node.error; break;
  case SCF_CALC: delete node.calc; break;
  case SCF_SELECT: delete node.selection; break;
  case SCF_INSERT: delete node.insertion; break;
  case SCF_UPDATE: delete node.update; break;
  case SCF_DELETE: delete node.deletion; break;
  case SCF_CREATE_TABLE: delete node.create_table; break;
  case SCF_CREATE_VIEW: delete node.create_view; break;
  case SCF_DROP_TABLE: delete node.drop_table; break;
  case SCF_CREATE_INDEX: delete node.create_index; break;
  case SCF_DROP_INDEX: delete node.drop_index; break;
  case SCF_DESC_TABLE: delete node.desc_table; break;
  case SCF_LOAD_DATA: delete node.load_data; break;
  case SCF_EXPLAIN: delete node.explain; break;
  case SCF_SET_VARIABLE: delete node.set_variable; break;
  default:;
  }
}

////////////////////////////////////////////////////////////////////////////////

int sql_parse(const char *st, ParsedSqlResult *sql_result);

RC parse(const char *st, ParsedSqlResult *sql_result) {
  sql_parse(st, sql_result);
  return RC::SUCCESS;
}

CreateTableSqlNode::~CreateTableSqlNode() {
  if (select)
    delete select;
}

CreateViewSqlNode::~CreateViewSqlNode() {
  if (select)
    delete select;
}
