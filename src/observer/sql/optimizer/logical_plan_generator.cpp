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

#include "common/log/log.h"
#include "common/rc.h"
#include "sql/operator/aggregate_logical_operator.h"
#include "sql/operator/cached_logical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/create_table_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/rename_logical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/sub_query_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/view_get_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/join_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/update_stmt.h"
#include <memory>
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

  case StmtType::UPDATE: {
    UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);
    rc = create_plan(update_stmt, logical_operator);
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
  case StmtType::CREATE_TABLE: {
    CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);
    rc = create_plan(create_table_stmt, logical_operator);
  } break;
  default: {
    rc = RC::UNIMPLENMENT;
  }
  }
  if (rc == RC::SUCCESS) {
    // 生成所有逻辑计划的依赖表
    std::function<void(LogicalOperator *)> dfs = [&](LogicalOperator *oper) {
      for (auto &x : oper->children()) {
        dfs(x.get());
      }
      oper->gen_child_tables();
      oper->add_current_table();
    };
    dfs(logical_operator.get());
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator) {
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(JoinStmt *join_stmt, const set<Field> &fields,
                                     std::unique_ptr<LogicalOperator> &logical_operator, bool readonly) {
  unique_ptr<LogicalOperator> table_oper;
  vector<Field> current_fields;
  Table *table = join_stmt->table();
  for (auto field : fields) {
    if (strcmp(table->name(), field.table_name()) == 0) {
      current_fields.push_back(field);
    }
  }
  RC rc = RC::SUCCESS;
  rc = get_table_get_operator(table, current_fields, readonly, table_oper);
  if (rc != RC::SUCCESS)
    return rc;
  if (table->name() != join_stmt->alias()) {
    // TODO(zhaoyiping): 改名
    auto *rename = new RenameLogicalOperator;
    rename->add_child(std::move(table_oper));
    auto &table_meta = table->table_meta();
    for (int i = table_meta.sys_field_num(); i < table_meta.field_num(); i++) {
      auto field_meta = table_meta.field(i);
      rename->add_rename(TupleCellSpec(join_stmt->alias().c_str(), field_meta->name()),
                         TupleCellSpec(table->name(), field_meta->name()));
    }
    for (auto &x : current_fields) {
    }
    table_oper.reset(rename);
  }
  if (join_stmt->sub_join() == nullptr) {
    logical_operator.swap(table_oper);
    return rc;
  }
  unique_ptr<LogicalOperator> sub_op;
  rc = create_plan(join_stmt->sub_join().get(), fields, sub_op);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  JoinLogicalOperator *join_oper = new JoinLogicalOperator;
  join_oper->add_child(std::move(sub_op));
  join_oper->add_child(std::move(table_oper));
  if (join_stmt->condition() == nullptr) {
    logical_operator.reset(join_oper);
  } else {
    logical_operator.reset(new PredicateLogicalOperator(std::move(join_stmt->condition())));
    logical_operator->add_child(unique_ptr<LogicalOperator>(join_oper));
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator,
                                     bool readonly) {
  unique_ptr<LogicalOperator> table_oper(nullptr);

  // const auto &current_tables = select_stmt->current_tables();
  const std::set<Field> &all_fields = select_stmt->used_fields();

  RC rc = RC::SUCCESS;

  if (select_stmt->join_stmt()) {
    rc = create_plan(select_stmt->join_stmt().get(), all_fields, table_oper, readonly);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  if (select_stmt->sub_queries().size()) {
    unique_ptr<SubQueryLogicalOperator> sub_query_operator(new SubQueryLogicalOperator(table_oper));
    for (auto &x : select_stmt->sub_queries()) {
      unique_ptr<LogicalOperator> sub_query;
      rc = create_plan(x->stmt().get(), sub_query);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to generate sub query");
        return rc;
      }
      if (!x->stmt()->use_father()) {
        unique_ptr<LogicalOperator> cached_opeartor(new CachedLogicalOperator(sub_query));
        sub_query.swap(cached_opeartor);
      }
      sub_query_operator->add_sub_query(std::move(sub_query), x->name());
    }
    table_oper.reset(sub_query_operator.release());
  }

  unique_ptr<LogicalOperator> predicate_oper;
  auto &filter_stmt = select_stmt->filter_stmt();
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
    AggregateLogicalOperator *aggr_oper = new AggregateLogicalOperator(select_stmt->aggregation_stmt().get());
    if (scan_oper) {
      aggr_oper->add_child(std::move(scan_oper));
    }
    scan_oper.reset(aggr_oper);
  }

  auto &having_stmt = select_stmt->having_stmt();
  if (having_stmt) {
    predicate_oper.reset(new PredicateLogicalOperator(std::move(having_stmt->filter_expr())));
    predicate_oper->add_child(std::move(scan_oper));
    scan_oper.swap(predicate_oper);
  }

  project_oper->add_child(std::move(scan_oper));
  scan_oper = std::move(project_oper);

  auto &orderby_stmt = select_stmt->orderby_stmt();
  if (orderby_stmt->order_by_units().size()) {
    unique_ptr<SortLogicalOperator> sort_operator(new SortLogicalOperator);
    for (auto &x : orderby_stmt->order_by_units()) {
      sort_operator->add_order_unit(std::move(x));
    }
    sort_operator->set_schema(select_stmt->schema());
    sort_operator->add_child(std::move(scan_oper));
    scan_oper = std::move(sort_operator);
  }

  logical_operator.swap(scan_oper);
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
  unique_ptr<LogicalOperator> table_get_oper;
  RC rc = RC::SUCCESS;
  rc = get_table_get_operator(table, fields, false /*readonly*/, table_get_oper);
  if (rc != RC::SUCCESS)
    return rc;

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

RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, std::unique_ptr<LogicalOperator> &logical_operator) {
  RC rc = RC::SUCCESS;
  Table *table = update_stmt->table();
  unique_ptr<LogicalOperator> table_get_oper;
  std::vector<Field> fields;
  for (auto &x : update_stmt->units()) {
    fields.push_back(x.field);
  }
  rc = get_table_get_operator(table, fields, false, table_get_oper);
  if (rc != RC::SUCCESS)
    return rc;
  auto &sub_queries = update_stmt->sub_queries();
  if (sub_queries.size()) {
    auto *sub_query_operator = new SubQueryLogicalOperator(table_get_oper);
    for (auto &sub_query : sub_queries) {
      std::unique_ptr<LogicalOperator> oper;
      rc = create(sub_query.get()->stmt().get(), oper);
      if (rc != RC::SUCCESS)
        return rc;
      sub_query_operator->add_sub_query(std::move(oper), sub_query->name());
    }
    table_get_oper.reset(sub_query_operator);
  }
  auto *filter_stmt = update_stmt->filter();
  if (filter_stmt != nullptr) {
    auto *predicate_oper = new PredicateLogicalOperator(std::move(filter_stmt->filter_expr()));
    predicate_oper->add_child(std::move(table_get_oper));
    table_get_oper.reset(predicate_oper);
  }
  logical_operator.reset(new UpdateLogicalOperator(table, update_stmt->units()));
  logical_operator->add_child(std::move(table_get_oper));
  return rc;
}
RC LogicalPlanGenerator::create_plan(CreateTableStmt *create_table_stmt,
                                     std::unique_ptr<LogicalOperator> &logical_operator) {
  RC rc = RC::SUCCESS;
  auto &select_stmt = create_table_stmt->select_stmt();
  CreateTableLogicalOperator *oper = new CreateTableLogicalOperator;
  std::unique_ptr<LogicalOperator> select_oper;
  if (select_stmt != nullptr) {
    oper->schema_ = select_stmt->schema();
    oper->types_ = select_stmt->types();
    rc = create(select_stmt.get(), select_oper);
    if (rc != RC::SUCCESS)
      return rc;
    oper->add_child(std::move(select_oper));
  }
  oper->table_name_ = create_table_stmt->table_name();
  oper->attr_infos_ = create_table_stmt->attr_infos();
  oper->db_ = create_table_stmt->db_;
  logical_operator.reset(oper);
  return rc;
}

RC LogicalPlanGenerator::get_table_get_operator(Table *table, std::vector<Field> fields, bool readonly,
                                                std::unique_ptr<LogicalOperator> &logical_operator) {
  if (table->view() == nullptr) {
    logical_operator.reset(new TableGetLogicalOperator(table, fields, readonly));
    return RC::SUCCESS;
  }
  // TODO(zhaoyiping): 这里想一下怎么处理order by
  auto *view = table->view();
  std::unique_ptr<SelectStmt> select;
  RC rc = RC::SUCCESS;
  rc = view->select(select);
  if (rc != RC::SUCCESS)
    return rc;
  logical_operator.reset(new ViewGetLogicalOperator(view, fields, readonly, select.get()));
  std::unique_ptr<LogicalOperator> select_oper;
  rc = create(select.get(), select_oper);
  if (rc != RC::SUCCESS)
    return rc;
  logical_operator->add_child(std::move(select_oper));
  return RC::SUCCESS;
}
