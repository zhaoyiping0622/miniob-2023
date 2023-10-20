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
// Created by Wangyunlai on 2022/12/30.
//

#include "sql/optimizer/predicate_pushdown_rewriter.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/parser/parse_defs.h"
#include <utility>

RC PredicatePushdownRewriter::rewrite(unique_ptr<LogicalOperator> &oper, bool &change_made) {
  RC rc = RC::SUCCESS;
  if (oper->type() != LogicalOperatorType::PREDICATE) {
    return rc;
  }
  auto *predicate_operator = static_cast<PredicateLogicalOperator *>(oper.get());
  auto &expressions = predicate_operator->expressions();
  if (expressions.size() != 1) {
    LOG_ERROR("predicate operator should has one expression");
    return RC::INTERNAL;
  }
  vector<unique_ptr<Expression> *> units;
  get_compare_units(&expressions[0], units);
  for (auto &child : oper->children()) {
    for (auto &unit : units) {
      apply_expression(nullptr, child, *unit, change_made);
    }
  }
  return rc;
}

RC PredicatePushdownRewriter::get_compare_units(unique_ptr<Expression> *expr, vector<unique_ptr<Expression> *> &units) {
  RC rc = RC::SUCCESS;
  if ((*expr)->type() == ExprType::COMPARISON) {
    units.push_back(expr);
    return rc;
  }
  if ((*expr)->type() == ExprType::CONJUNCTION) {
    auto *conjunction_expr = static_cast<ConjunctionExpr *>(expr->get());
    auto conjunction_type = conjunction_expr->conjunction_type();
    if (conjunction_type == ConjunctionType::OR || conjunction_type == ConjunctionType::SINGLE) {
      units.push_back(expr);
      return rc;
    }
    if ((rc = get_compare_units(&conjunction_expr->left(), units)) != RC::SUCCESS ||
        (rc = get_compare_units(&conjunction_expr->right(), units)) != RC::SUCCESS) {
      return rc;
    }
    return rc;
  }
  return rc;
}

RC PredicatePushdownRewriter::apply_expression(LogicalOperator *father, std::unique_ptr<LogicalOperator> &oper,
                                               std::unique_ptr<Expression> &expr, bool &change_made) {
  RC rc = RC::SUCCESS;
  if (oper->type() == LogicalOperatorType::AGGREGATE) {
    // 过滤表达式无法穿透聚合
    return rc;
  }
  auto &tables = oper->tables();
  auto fields = expr->reference_fields();
  for (auto field : fields) {
    if (tables.count(field.table_name()) == 0) {
      // 这个expr不能否仅由oper处理，无法下推
      return rc;
    }
  }
  for (auto &child : oper->children()) {
    // 将expr下推到子表达式
    rc = apply_expression(oper.get(), child, expr, change_made);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("failed to apply expression");
      return rc;
    }
    // 下层已经处理了这个表达式
    if (expr->type() == ExprType::VALUE) {
      return rc;
    }
  }
  if (oper->type() == LogicalOperatorType::TABLE_GET) {
    auto *table_get_oper = static_cast<TableGetLogicalOperator *>(oper.get());
    table_get_oper->add_predicate(std::move(expr));
  } else if (oper->type() == LogicalOperatorType::RENAME) {
    // 不能穿透rename
    return rc;
  } else if (father == nullptr || father->type() == LogicalOperatorType::PREDICATE) {
    // 当前层是最高层，或者上一层就有过滤表达式
    return rc;
  } else if (oper->type() == LogicalOperatorType::PREDICATE) {
    auto *predicate_oper = static_cast<PredicateLogicalOperator *>(oper.get());
    ConjunctionExpr *conjunction_expr =
        new ConjunctionExpr(ConjunctionType::AND, expr.release(), predicate_oper->expressions()[0].release());
    predicate_oper->expressions()[0].reset(conjunction_expr);
  } else {
    PredicateLogicalOperator *predicate_oper = new PredicateLogicalOperator(std::move(expr));
    predicate_oper->add_child(unique_ptr<LogicalOperator>(oper.release()));
    predicate_oper->gen_child_tables();
    oper.reset(predicate_oper);
  }
  change_made = true;
  expr.reset(new ValueExpr(Value(true)));
  return rc;
}
