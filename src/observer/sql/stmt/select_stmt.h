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
#include "sql/stmt/aggregation_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"

class FieldMeta;
class FilterStmt;
class Db;
class Table;

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
  static RC create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt);

public:
  const std::vector<Table *> &tables() const { return tables_; }
  const std::set<Field> &used_fields() const { return used_fields_; }
  const std::vector<std::unique_ptr<Expression>> &expressions() const { return expressions_; }
  std::vector<std::unique_ptr<Expression>> &expressions() { return expressions_; }
  const std::vector<std::set<Field>> reference_fields() const { return reference_fields_; }
  std::unique_ptr<FilterStmt> &filter_stmt() { return filter_stmt_; }
  std::unique_ptr<FilterStmt> &having_stmt() { return having_stmt_; }
  std::unique_ptr<OrderByStmt> &orderby_stmt() { return orderby_stmt_; }
  const std::shared_ptr<TupleSchema> &schema() const { return schema_; }
  const std::unique_ptr<AggregationStmt> &aggregation_stmt() const { return aggregation_stmt_; }

private:
  std::set<Field> used_fields_;
  std::vector<std::set<Field>> reference_fields_;
  std::vector<std::unique_ptr<Expression>> expressions_;
  std::vector<Table *> tables_;
  std::shared_ptr<TupleSchema> schema_;
  std::unique_ptr<FilterStmt> filter_stmt_;
  std::unique_ptr<FilterStmt> having_stmt_;
  std::unique_ptr<AggregationStmt> aggregation_stmt_ = nullptr;
  std::unique_ptr<OrderByStmt> orderby_stmt_;
};
