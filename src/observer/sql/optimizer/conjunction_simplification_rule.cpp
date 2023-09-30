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
// Created by Wangyunlai on 2022/12/26.
//

#include "sql/optimizer/conjunction_simplification_rule.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include <utility>

RC try_to_get_bool_constant(std::unique_ptr<Expression> &expr, bool &constant_value) {
  if (expr->type() == ExprType::VALUE && expr->value_type() == BOOLEANS) {
    auto value_expr = static_cast<ValueExpr *>(expr.get());
    constant_value = value_expr->get_value().get_boolean();
    return RC::SUCCESS;
  }
  return RC::INTERNAL;
}
RC ConjunctionSimplificationRule::rewrite(std::unique_ptr<Expression> &expr, bool &change_made) {
  RC rc = RC::SUCCESS;
  if (expr->type() != ExprType::CONJUNCTION) {
    return rc;
  }

  change_made = false;
  auto conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());
  auto &left = conjunction_expr->left();
  auto &right = conjunction_expr->right();
  auto type = conjunction_expr->conjunction_type();
  if (type == ConjunctionType::SINGLE) {
    LOG_ERROR("ConjunctionType::SINGLE exist in expresion");
    return RC::INTERNAL;
  } else {
    bool vl, vr;
    RC rc1 = try_to_get_bool_constant(left, vl);
    RC rc2 = try_to_get_bool_constant(right, vr);
    if (rc1 != RC::SUCCESS && rc2 != RC::SUCCESS) {
      return RC::SUCCESS;
    } else if (rc1 == RC::SUCCESS && rc2 == RC::SUCCESS) {
      change_made = true;
      if (type == ConjunctionType::AND) {
        expr.reset(new ValueExpr(Value(vl && vr)));
      } else if (type == ConjunctionType::OR) {
        expr.reset(new ValueExpr(Value(vl || vr)));
      }
    } else {
      change_made = true;
      if (rc1 != RC::SUCCESS) {
        swap(vl, vr);
        swap(left, right);
      }
      if (type == ConjunctionType::AND) {
        if (vl) {
          expr.reset(right.release());
        } else {
          expr.reset(new ValueExpr(Value(false)));
        }
      } else if (type == ConjunctionType::OR) {
        if (vl) {
          expr.reset(new ValueExpr(Value(true)));
        } else {
          expr.reset(right.release());
        }
      }
    }
  }
  return RC::SUCCESS;
}
