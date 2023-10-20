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
// Created by Wangyunlai on 2022/12/14.
//

#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/operator/aggregate_logical_operator.h"
#include "sql/operator/aggregate_physical_operator.h"
#include "sql/operator/cached_logical_operator.h"
#include "sql/operator/cached_physical_operator.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/calc_physical_operator.h"
#include "sql/operator/create_table_logical_operator.h"
#include "sql/operator/create_table_physical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/delete_physical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/explain_physical_operator.h"
#include "sql/operator/index_scan_physical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/insert_physical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/join_physical_operator.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/predicate_physical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/rename_logical_operator.h"
#include "sql/operator/rename_physical_operator.h"
#include "sql/operator/sort_logical_operator.h"
#include "sql/operator/sort_physical_operator.h"
#include "sql/operator/sub_query_logical_operator.h"
#include "sql/operator/sub_query_physical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/table_scan_physical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/update_physical_operator.h"
#include "sql/operator/view_get_logical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/parser/parse_defs.h"
#include "storage/index/index.h"

using namespace std;

RC PhysicalPlanGenerator::create(LogicalOperator &logical_operator, unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;

  switch (logical_operator.type()) {
  case LogicalOperatorType::CALC: {
    return create_plan(static_cast<CalcLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::TABLE_GET: {
    return create_plan(static_cast<TableGetLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::VIEW_GET: {
    return create_plan(static_cast<ViewGetLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::PREDICATE: {
    return create_plan(static_cast<PredicateLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::PROJECTION: {
    return create_plan(static_cast<ProjectLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::INSERT: {
    return create_plan(static_cast<InsertLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::UPDATE: {
    return create_plan(static_cast<UpdateLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::DELETE: {
    return create_plan(static_cast<DeleteLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::EXPLAIN: {
    return create_plan(static_cast<ExplainLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::JOIN: {
    return create_plan(static_cast<JoinLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::AGGREGATE: {
    return create_plan(static_cast<AggregateLogicalOperator &>(logical_operator), oper);
  } break;

  case LogicalOperatorType::SORT: {
    return create_plan(static_cast<SortLogicalOperator &>(logical_operator), oper);
  } break;
  case LogicalOperatorType::SUB_QUERY: {
    return create_plan(static_cast<SubQueryLogicalOperator &>(logical_operator), oper);
  } break;
  case LogicalOperatorType::CACHED: {
    return create_plan(static_cast<CachedLogicalOperator &>(logical_operator), oper);
  } break;
  case LogicalOperatorType::CREATE_TABLE: {
    return create_plan(static_cast<CreateTableLogicalOperator &>(logical_operator), oper);
  } break;
  case LogicalOperatorType::RENAME: {
    return create_plan(static_cast<RenameLogicalOperator &>(logical_operator), oper);
  } break;

  default: {
    return RC::INVALID_ARGUMENT;
  }
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(TableGetLogicalOperator &table_get_oper, unique_ptr<PhysicalOperator> &oper) {
  vector<unique_ptr<Expression>> &predicates = table_get_oper.predicates();
  // 看看是否有可以用于索引查找的表达式
  Table *table = table_get_oper.table();

  Index *index = nullptr;
  std::vector<std::pair<Field, Value>> field_values;
  for (auto &expr : predicates) {
    if (expr->type() == ExprType::COMPARISON) {
      ValueExpr *value_expr = nullptr;
      auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
      // 简单处理，就找等值查询
      if (comparison_expr->comp() != EQUAL_TO) {
        continue;
      }

      unique_ptr<Expression> &left_expr = comparison_expr->left();
      unique_ptr<Expression> &right_expr = comparison_expr->right();
      // 左右比较的一边最少是一个值
      if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) {
        continue;
      }

      FieldExpr *field_expr = nullptr;
      if (left_expr->type() == ExprType::FIELD) {
        ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
        field_expr = static_cast<FieldExpr *>(left_expr.get());
        value_expr = static_cast<ValueExpr *>(right_expr.get());
      } else if (right_expr->type() == ExprType::FIELD) {
        ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
        field_expr = static_cast<FieldExpr *>(right_expr.get());
        value_expr = static_cast<ValueExpr *>(left_expr.get());
      }

      if (field_expr == nullptr) {
        continue;
      }

      const Field &field = field_expr->field();
      Value value;
      ASSERT(value_expr != nullptr, "got an index but value expr is null ?");
      if (value_expr->try_get_value(value) != RC::SUCCESS)
        continue;
      field_values.push_back({field, value});
    }
  }
  // TODO(zhaoyiping)
  // 这里要研究一下怎么确定用哪个索引

  std::vector<const char *> fields;
  for (auto &[f, value] : field_values) {
    fields.push_back(f.field_name());
  }
  index = table->find_index_by_fields(fields);

  if (index != nullptr) {
    // 构建value
    std::vector<Value> values;
    auto &index_meta = index->index_meta();
    for (auto &field : index_meta.fields()) {
      bool found = false;
      for (auto &[f, value] : field_values) {
        if (strcmp(f.field_name(), field.name()) == 0) {
          found = true;
          values.push_back(value);
          break;
        }
      }
      if (!found) {
        break;
      }
    }
    IndexScanPhysicalOperator *index_scan_oper = new IndexScanPhysicalOperator(
        table, index, table_get_oper.readonly(), values, true /*left_inclusive*/, values, true /*right_inclusive*/);

    index_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(index_scan_oper);
    LOG_TRACE("use index scan");
  } else {
    auto table_scan_oper = new TableScanPhysicalOperator(table, table_get_oper.readonly());
    table_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(table_scan_oper);
    LOG_TRACE("use table scan");
  }

  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(PredicateLogicalOperator &pred_oper, unique_ptr<PhysicalOperator> &oper) {
  vector<unique_ptr<LogicalOperator>> &children_opers = pred_oper.children();
  ASSERT(children_opers.size() == 1, "predicate logical operator's sub oper number should be 1");

  LogicalOperator &child_oper = *children_opers.front();

  unique_ptr<PhysicalOperator> child_phy_oper;
  RC rc = create(child_oper, child_phy_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create child operator of predicate operator. rc=%s", strrc(rc));
    return rc;
  }

  vector<unique_ptr<Expression>> &expressions = pred_oper.expressions();
  ASSERT(expressions.size() == 1, "predicate logical operator's children should be 1");

  unique_ptr<Expression> expression = std::move(expressions.front());
  oper = unique_ptr<PhysicalOperator>(new PredicatePhysicalOperator(std::move(expression)));
  oper->add_child(std::move(child_phy_oper));
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ProjectLogicalOperator &project_oper, unique_ptr<PhysicalOperator> &oper) {
  vector<unique_ptr<LogicalOperator>> &child_opers = project_oper.children();

  unique_ptr<PhysicalOperator> child_phy_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc = create(*child_oper, child_phy_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  ProjectPhysicalOperator *project_operator = new ProjectPhysicalOperator;
  vector<unique_ptr<Expression>> &expressions = project_oper.expressions();
  for (auto &expr : expressions) {
    if (expr->type() == ExprType::FIELD) {
      Field field = *expr->reference_fields().begin();
      project_operator->add_projection(field.table(), field.meta());
    } else {
      project_operator->add_projection(expr->name().c_str());
    }
    project_operator->add_expression(expr);
  }

  if (child_phy_oper) {
    project_operator->add_child(std::move(child_phy_oper));
  }

  oper = unique_ptr<PhysicalOperator>(project_operator);

  LOG_TRACE("create a project physical operator");
  return rc;
}

RC PhysicalPlanGenerator::create_plan(InsertLogicalOperator &insert_oper, unique_ptr<PhysicalOperator> &oper) {
  Table *table = insert_oper.table();
  vector<vector<Value>> &values = insert_oper.values();
  InsertPhysicalOperator *insert_phy_oper = new InsertPhysicalOperator(table, std::move(values));
  oper.reset(insert_phy_oper);
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(DeleteLogicalOperator &delete_oper, unique_ptr<PhysicalOperator> &oper) {
  vector<unique_ptr<LogicalOperator>> &child_opers = delete_oper.children();

  unique_ptr<PhysicalOperator> child_physical_oper;

  RC rc = RC::SUCCESS;
  if (!child_opers.empty()) {
    LogicalOperator *child_oper = child_opers.front().get();
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
      return rc;
    }
  }

  oper = unique_ptr<PhysicalOperator>(new DeletePhysicalOperator(delete_oper.table()));

  if (child_physical_oper) {
    oper->add_child(std::move(child_physical_oper));
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ExplainLogicalOperator &explain_oper, unique_ptr<PhysicalOperator> &oper) {
  vector<unique_ptr<LogicalOperator>> &child_opers = explain_oper.children();

  RC rc = RC::SUCCESS;
  unique_ptr<PhysicalOperator> explain_physical_oper(new ExplainPhysicalOperator);
  for (unique_ptr<LogicalOperator> &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create child physical operator. rc=%s", strrc(rc));
      return rc;
    }

    explain_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(explain_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(JoinLogicalOperator &join_oper, unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;

  vector<unique_ptr<LogicalOperator>> &child_opers = join_oper.children();
  if (child_opers.size() != 2) {
    LOG_WARN("join operator should have 2 children, but have %d", child_opers.size());
    return RC::INTERNAL;
  }

  unique_ptr<PhysicalOperator> join_physical_oper(new NestedLoopJoinPhysicalOperator);
  for (auto &child_oper : child_opers) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create physical child oper. rc=%s", strrc(rc));
      return rc;
    }

    join_physical_oper->add_child(std::move(child_physical_oper));
  }

  oper = std::move(join_physical_oper);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(CalcLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;
  CalcPhysicalOperator *calc_oper = new CalcPhysicalOperator(std::move(logical_oper.expressions()));
  oper.reset(calc_oper);
  return rc;
}
RC PhysicalPlanGenerator::create_plan(AggregateLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  unique_ptr<PhysicalOperator> child;
  RC rc = create(*logical_oper.children()[0], child);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  auto ret =
      std::make_unique<AggregatePhysicalOperator>(logical_oper.group_fields(), logical_oper.aggregation_units(), child);
  oper = std::move(ret);
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(SortLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  if (logical_oper.children().size() != 1) {
    LOG_ERROR("sort logical operator should have one child");
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> child_oper;
  RC rc = RC::SUCCESS;
  rc = create(*logical_oper.children()[0], child_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  auto &schema = logical_oper.schema();
  auto sort_operator = make_unique<SortPhysicalOperator>();
  sort_operator->add_child(std::move(child_oper));
  sort_operator->orders_ = logical_oper.orders();
  sort_operator->schema_ = schema;
  auto &expressions = logical_oper.expressions();
  for (int i = 0; i < expressions.size(); i++) {
    if (expressions[i]->type() == ExprType::FIELD) {
      sort_operator->sort_speces_.emplace_back(static_cast<FieldExpr *>(expressions[i].get())->field());
    } else {
      sort_operator->sort_speces_.emplace_back(expressions[i]->name().c_str());
    }
  }
  oper.reset(sort_operator.release());
  return RC::SUCCESS;
}
RC PhysicalPlanGenerator::create_plan(SubQueryLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  auto &children = logical_oper.children();
  if (children.empty()) {
    LOG_ERROR("sub query should have at least 1 child");
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> tmp;
  RC rc = RC::SUCCESS;
  rc = create(*logical_oper.main_oper(), tmp);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  auto *sub_query = new SubQueryPhysicalOperator(std::move(tmp));
  for (int i = 0; i < children.size(); i++) {
    rc = create(*children[i], tmp);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    sub_query->add_sub_query(std::move(tmp), logical_oper.names()[i]);
  }
  oper.reset(sub_query);
  return rc;
}

RC PhysicalPlanGenerator::create_plan(CachedLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  if (logical_oper.children().size() != 1) {
    LOG_ERROR("cached logical operator should have one child");
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> child;
  RC rc = RC::SUCCESS;
  rc = create(*logical_oper.children()[0], child);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  oper.reset(new CachedPhysicalOperator(std::move(child)));
  return rc;
}

RC PhysicalPlanGenerator::create_plan(UpdateLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  UpdatePhysicalOperator *op = new UpdatePhysicalOperator;
  op->units_.swap(logical_oper.units());
  op->table_ = logical_oper.table();
  oper.reset(op);
  for (auto &child : logical_oper.children()) {
    std::unique_ptr<PhysicalOperator> cop;
    RC rc = create(*child, cop);
    if (rc != RC::SUCCESS)
      return rc;
    op->add_child(std::move(cop));
  }
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(CreateTableLogicalOperator &logical_oper,
                                      std::unique_ptr<PhysicalOperator> &oper) {
  CreateTablePhysicalOperator *op = new CreateTablePhysicalOperator;
  oper.reset(op);
  for (auto &child : logical_oper.children()) {
    std::unique_ptr<PhysicalOperator> cop;
    RC rc = create(*child, cop);
    if (rc != RC::SUCCESS)
      return rc;
    op->add_child(std::move(cop));
  }
  op->db_ = logical_oper.db_;
  op->schema_ = logical_oper.schema_;
  op->table_name_ = logical_oper.table_name_;
  op->attr_infos_ = logical_oper.attr_infos_;
  op->types_ = logical_oper.types_;
  return RC::SUCCESS;
}
RC PhysicalPlanGenerator::create_plan(ViewGetLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  auto &children = logical_oper.children();
  if (children.size() != 1) {
    return RC::INTERNAL;
  }
  RC rc = create(*children[0], oper);
  if (rc != RC::SUCCESS)
    return rc;
  View *view = logical_oper.view_;
  auto &metas = view->view_meta();
  auto *schema = logical_oper.schema_.get();
  std::vector<std::pair<TupleCellSpec, TupleCellSpec>> spec_map;
  for (int i = 0; i < schema->cell_num(); i++) {
    const TupleCellSpec &lower_spec = schema->cell_at(i);
    auto &meta = metas.metas()[i];
    spec_map.emplace_back(TupleCellSpec(meta.name().c_str()), lower_spec);
    spec_map.emplace_back(TupleCellSpec(view->name().c_str(), meta.name().c_str()), lower_spec);
  }
  auto *rename = new RenamePhysicalOperator(spec_map);
  rename->add_child(std::move(oper));
  oper.reset(rename);
  return RC::SUCCESS;
}
RC PhysicalPlanGenerator::create_plan(RenameLogicalOperator &logical_oper, std::unique_ptr<PhysicalOperator> &oper) {
  auto &children = logical_oper.children();
  if (children.size() != 1) {
    return RC::INTERNAL;
  }
  std::unique_ptr<PhysicalOperator> child_oper;
  RC rc = create(*children[0], child_oper);
  if (rc != RC::SUCCESS)
    return rc;
  oper.reset(new RenamePhysicalOperator(logical_oper.spec_map_));
  oper->add_child(std::move(child_oper));
  return RC::SUCCESS;
}
