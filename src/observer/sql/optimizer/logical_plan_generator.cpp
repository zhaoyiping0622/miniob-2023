/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include "sql/operator/aggregate_logical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include <utility>

using namespace std;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator) {
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
  case StmtType::CALC: {
    CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);
    rc = create_plan(calc_stmt, logical_operator);
  } break;

  case StmtType::SELECT: {
    SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
    rc = create_plan(select_stmt, logical_operator);
  } break;

  case StmtType::INSERT: {
    InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);
    rc = create_plan(insert_stmt, logical_operator);
  } break;

  case StmtType::DELETE: {
    DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);
    rc = create_plan(delete_stmt, logical_operator);
  } break;

  case StmtType::EXPLAIN: {
    ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);
    rc = create_plan(explain_stmt, logical_operator);
  } break;
  default: {
    rc = RC::UNIMPLENMENT;
  }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator) {
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator) {
  unique_ptr<LogicalOperator> table_oper(nullptr);

  const std::vector<Table *> &tables = select_stmt->tables();
  const std::set<Field> &all_fields = select_stmt->used_fields();
  for (Table *table : tables) {
    std::vector<Field> fields;
    for (const Field &field : all_fields) {
      if (0 == strcmp(field.table_name(), table->name())) {
        fields.push_back(field);
      }
    }

    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, true /*readonly*/));
    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  unique_ptr<LogicalOperator> predicate_oper;
  FilterStmt *filter_stmt = select_stmt->filter_stmt();
  if (filter_stmt != nullptr) {
    predicate_oper.reset(new PredicateLogicalOperator(std::move(filter_stmt->filter_expr())));
  }

  unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(select_stmt->expressions()));
  unique_ptr<LogicalOperator> scan_oper;
  if (predicate_oper) {
    if (table_oper) {
      predicate_oper->add_child(std::move(table_oper));
    }
    scan_oper.swap(predicate_oper);
  } else {
    if (table_oper) {
      scan_oper.swap(table_oper);
    }
  }

  if (select_stmt->aggregation_stmt()->has_aggregate()) {
    // FIXME(zhaoyiping): 这里要考虑一下如何处理table_oper是nullptr的情况
    AggregateLogicalOperator* aggr_oper = new AggregateLogicalOperator(select_stmt->aggregation_stmt().get());
    aggr_oper->add_child(std::move(scan_oper));
    scan_oper.reset(aggr_oper);
  }

  project_oper->add_child(std::move(scan_oper));

  logical_operator.swap(project_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator) {
  Table *table = insert_stmt->table();
  vector<vector<Value>> records = insert_stmt->records();

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, records);
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator) {
  Table *table = delete_stmt->table();
  FilterStmt *filter_stmt = delete_stmt->filter_stmt();
  std::vector<Field> fields;
  for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(i);
    fields.push_back(Field(table, field_meta));
  }
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false /*readonly*/));

  unique_ptr<LogicalOperator> predicate_oper;
  if (filter_stmt != nullptr) {
    predicate_oper.reset(new PredicateLogicalOperator(std::move(filter_stmt->filter_expr())));
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator) {
  Stmt *child_stmt = explain_stmt->child();
  unique_ptr<LogicalOperator> child_oper;
  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}
