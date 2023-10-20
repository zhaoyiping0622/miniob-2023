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
// Created by Wangyunlai on 2022/6/5.
//

#pragma once

#include <memory>
#include <vector>

#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/parser/value.h"
#include "sql/stmt/aggregation_stmt.h"
#include "sql/stmt/join_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/sub_query_stmt.h"
#include "storage/field/field.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;

struct FieldInfo {
  AttrType type;
  int length = 0;
  bool nullable = true;
  Field raw_field;
};

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class SelectStmt : public Stmt {
public:
  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override { return StmtType::SELECT; }

public:
  static RC create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt,
                   const std::unordered_map<std::string, Table *> *father_tables, std::set<Field> &father_fields);

public:
  const std::unordered_map<std::string, Table *> &current_tables() const { return current_tables_; }
  const std::set<Field> &used_fields() const { return used_fields_; }
  const std::vector<std::unique_ptr<Expression>> &expressions() const { return expressions_; }
  std::vector<std::unique_ptr<Expression>> &expressions() { return expressions_; }
  const std::vector<std::set<Field>> reference_fields() const { return reference_fields_; }
  std::unique_ptr<FilterStmt> &filter_stmt() { return filter_stmt_; }
  std::unique_ptr<FilterStmt> &having_stmt() { return having_stmt_; }
  std::unique_ptr<OrderByStmt> &orderby_stmt() { return orderby_stmt_; }
  std::unique_ptr<JoinStmt> &join_stmt() { return join_stmt_; }
  std::shared_ptr<TupleSchema> schema() { return schema_; }
  const std::unique_ptr<AggregationStmt> &aggregation_stmt() const { return aggregation_stmt_; }
  std::vector<std::unique_ptr<SubQueryStmt>> &sub_queries() { return sub_queries_; }
  bool use_father() const { return use_father_; }
  const std::vector<FieldInfo> &types() const { return types_; }
  std::string sql() { return sql_; }

private:
  std::unique_ptr<JoinStmt> join_stmt_;

  std::set<Field> used_fields_;                             // 当前查询会生成的scan中所有用到的field
  std::vector<std::set<Field>> reference_fields_;           // 每个表达式用到的field
  std::vector<std::unique_ptr<Expression>> expressions_;    // 所有表达式
  std::unordered_map<std::string, Table *> current_tables_; // 当前查询的表名
  std::unordered_map<std::string, Table *> father_tables_;  // 父节点的表名
  std::shared_ptr<TupleSchema> schema_;                     // 所有要输出的tuple的schema

  std::vector<std::unique_ptr<SubQueryStmt>> sub_queries_; // 子查询

  std::unique_ptr<FilterStmt> filter_stmt_; // where子句
  std::unique_ptr<FilterStmt> having_stmt_; // having子句

  std::unique_ptr<AggregationStmt> aggregation_stmt_; // 聚合

  std::unique_ptr<OrderByStmt> orderby_stmt_; // 排序

  std::vector<FieldInfo> types_;

  std::string sql_;

  bool use_father_ = false;
};

std::vector<AttrInfoSqlNode> get_result_descriptions(SelectStmt *select);
