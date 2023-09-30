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
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"

using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const {
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const {
  value = value_;
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type)
    : child_(std::move(child)), cast_type_(cast_type) {}
CastExpr::CastExpr(Expression *child, AttrType cast_type) : child_(child), cast_type_(cast_type) {}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const {
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }

  switch (cast_type_) {
  case BOOLEANS: {
    bool val = value.get_boolean();
    cast_value.set_boolean(val);
  } break;
  default: {
    rc = RC::INTERNAL;
    LOG_WARN("unsupported convert from type %d to %d", child_->value_type(), cast_type_);
  }
  }
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &cell) const {
  RC rc = child_->get_value(tuple, cell);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(cell, cell);
}

RC CastExpr::try_get_value(Value &value) const {
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, value);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right)) {}
ComparisonExpr::ComparisonExpr(CompOp comp, Expression *left, Expression *right)
    : comp_(comp), left_(left), right_(right) {}

ComparisonExpr::~ComparisonExpr() {}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const {
  RC rc = RC::SUCCESS;
  int cmp_result = left.compare(right);
  result = false;
  switch (comp_) {
  case EQUAL_TO: {
    result = (0 == cmp_result);
  } break;
  case LESS_EQUAL: {
    result = (cmp_result <= 0);
  } break;
  case NOT_EQUAL: {
    result = (cmp_result != 0);
  } break;
  case LESS_THAN: {
    result = (cmp_result < 0);
  } break;
  case GREAT_EQUAL: {
    result = (cmp_result >= 0);
  } break;
  case GREAT_THAN: {
    result = (cmp_result > 0);
  } break;
  default: {
    LOG_WARN("unsupported comparison. %d", comp_);
    rc = RC::INTERNAL;
  } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const {
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *left_value_expr = static_cast<ValueExpr *>(left_.get());
    ValueExpr *right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell = left_value_expr->get_value();
    const Value &right_cell = right_value_expr->get_value();

    bool value = false;
    RC rc = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const {
  Value left_value;
  Value right_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  bool bool_value = false;
  rc = compare_value(left_value, right_value, bool_value);
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(ConjunctionType type, Expression *left, Expression *right)
    : conjunction_type_(type), left_(left), right_(right) {}

ConjunctionExpr::ConjunctionExpr(ConjunctionType type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : conjunction_type_(type), left_(std::move(left)), right_(std::move(right)) {}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;
  if (left_ == nullptr && right_ == nullptr) {
    value.set_boolean(true);
    return rc;
  }

  rc = left_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value by left expression. rc=%s", strrc(rc));
    return rc;
  }

  bool left_value = value.get_boolean();

  switch (conjunction_type_) {
  case ConjunctionType::SINGLE: return rc;
  case ConjunctionType::AND: {
    if (!left_value) {
      value.set_boolean(left_value);
      return rc;
    }
    break;
  }
  case ConjunctionType::OR: {
    if (left_value) {
      value.set_boolean(left_value);
      return rc;
    }
    break;
  }
  }

  rc = right_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value by right expression. rc=%s", strrc(rc));
    return rc;
  }

  value.set_boolean(value.get_boolean());
  return RC::SUCCESS;
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticType type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right) {}
ArithmeticExpr::ArithmeticExpr(ArithmeticType type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right)) {}

AttrType ArithmeticExpr::value_type() const {
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != ArithmeticType::DIV) {
    return AttrType::INTS;
  }

  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const {
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();

  switch (arithmetic_type_) {
  case ArithmeticType::ADD: {
    if (target_type == AttrType::INTS) {
      value.set_int(left_value.get_int() + right_value.get_int());
    } else {
      value.set_float(left_value.get_float() + right_value.get_float());
    }
  } break;

  case ArithmeticType::SUB: {
    if (target_type == AttrType::INTS) {
      value.set_int(left_value.get_int() - right_value.get_int());
    } else {
      value.set_float(left_value.get_float() - right_value.get_float());
    }
  } break;

  case ArithmeticType::MUL: {
    if (target_type == AttrType::INTS) {
      value.set_int(left_value.get_int() * right_value.get_int());
    } else {
      value.set_float(left_value.get_float() * right_value.get_float());
    }
  } break;

  case ArithmeticType::DIV: {
    if (target_type == AttrType::INTS) {
      if (right_value.get_int() == 0) {
        // NOTE: 设置为整数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为整数最大值。
        value.set_int(numeric_limits<int>::max());
      } else {
        value.set_int(left_value.get_int() / right_value.get_int());
      }
    } else {
      if (right_value.get_float() > -EPSILON && right_value.get_float() < EPSILON) {
        // NOTE: 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
        value.set_float(numeric_limits<float>::max());
      } else {
        value.set_float(left_value.get_float() / right_value.get_float());
      }
    }
  } break;

  case ArithmeticType::NEGATIVE: {
    if (target_type == AttrType::INTS) {
      value.set_int(-left_value.get_int());
    } else {
      value.set_float(-left_value.get_float());
    }
  } break;

  default: {
    rc = RC::INTERNAL;
    LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
  } break;
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_value(left_value, right_value, value);
}

RC Expression::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                      const ExprSqlNode *expr_node, Expression *&expr) {
  RC rc = RC::SUCCESS;
  switch (expr_node->type()) {
  case ExprType::NONE: {
    LOG_ERROR("unable to translate NONE expr to Expression");
    return RC::INTERNAL;
  }
  case ExprType::STAR: {
    LOG_ERROR("unable to translate STAR expr to Expression");
    return RC::INTERNAL;
  }
  case ExprType::CAST: {
    LOG_ERROR("unable to translate CAST expr to Expression");
    return RC::INTERNAL;
  }
  case ExprType::FIELD: rc = FieldExpr::create(db, default_table, tables, expr_node->get_field(), expr); break;
  case ExprType::VALUE: rc = ValueExpr::create(expr_node->get_value(), expr); break;
  case ExprType::COMPARISON:
    rc = ComparisonExpr::create(db, default_table, tables, expr_node->get_comparison(), expr);
    break;
  case ExprType::CONJUNCTION:
    rc = ConjunctionExpr::create(db, default_table, tables, expr_node->get_conjunction(), expr);
    break;
  case ExprType::ARITHMETIC:
    rc = ArithmeticExpr::create(db, default_table, tables, expr_node->get_arithmetic(), expr);
    break;
  }
  if (rc != RC::SUCCESS)
    return rc;
  expr->set_name(expr_node->name());
  return RC::SUCCESS;
}

RC ArithmeticExpr::try_get_value(Value &value) const {
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                       const string &table_name, const string &field_name, Table *&table, const FieldMeta *&field) {
  if (common::is_blank(table_name.c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(table_name);
    if (iter != tables->end()) {
      table = iter->second;
    }
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", table_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(field_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), field_name.c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

RC FieldExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                     const FieldExprSqlNode *field_node, Expression *&expr) {
  string table_name = field_node->table_name;
  string field_name = field_node->field_name;
  Table *table;
  const FieldMeta *field;
  RC rc = get_table_and_field(db, default_table, tables, table_name, field_name, table, field);
  if (rc != RC::SUCCESS) {
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  expr = new FieldExpr(table, field);
  return RC::SUCCESS;
}

RC ValueExpr::create(const ValueExprSqlNode *value_node, Expression *&expr) {
  expr = new ValueExpr(value_node->value);
  return RC::SUCCESS;
}

RC CastExpr::create(AttrType target_type, Expression *&expr) {
  if (expr == nullptr) {
    LOG_ERROR("cast null expression");
    return RC::INTERNAL;
  }
  if (expr->value_type() == target_type)
    return RC::SUCCESS;
  if (expr->type() == ExprType::VALUE) {
    auto *value_expr = static_cast<ValueExpr *>(expr);
    Value value = value_expr->get_value();
    delete expr;
    if (!Value::convert(value.attr_type(), target_type, value)) {
      LOG_ERROR("cannot cast value %s from %s to %s", value.to_string().c_str(), attr_type_to_string(value.attr_type()),
                attr_type_to_string(target_type));
      return RC::INTERNAL;
    }
    expr = new ValueExpr(value);
    return RC::SUCCESS;
  }
  expr = new CastExpr(expr, target_type);
  return RC::SUCCESS;
}

RC ComparisonExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                          const ComparisonExprSqlNode *comparison_node, Expression *&expr) {
  RC rc = RC::SUCCESS;
  Expression *left, *right;
  rc = Expression::create(db, default_table, tables, comparison_node->left, left);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  Expression::create(db, default_table, tables, comparison_node->right, right);
  if (rc != RC::SUCCESS) {
    delete left;
    return rc;
  }
  AttrType target_type = AttrTypeCompare(left->value_type(), right->value_type());
  if (target_type == UNDEFINED || (rc = CastExpr::create(target_type, left)) != RC::SUCCESS ||
      (rc = CastExpr::create(target_type, right)) != RC::SUCCESS) {
    delete left;
    delete right;
    return RC::INTERNAL;
  };
  expr = new ComparisonExpr(comparison_node->op, left, right);
  return RC::SUCCESS;
}

RC ConjunctionExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                           const ConjunctionExprSqlNode *conjunction_node, Expression *&expr) {
  Expression *left, *right;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, conjunction_node->left, left);
  if (rc != RC::SUCCESS || (rc = CastExpr::create(BOOLEANS, left)) != RC::SUCCESS) {
    return rc;
  }
  if (conjunction_node->type != ConjunctionType::SINGLE) {
    rc = Expression::create(db, default_table, tables, conjunction_node->right, right);
    if (rc != RC::SUCCESS || (rc = CastExpr::create(BOOLEANS, right)) != RC::SUCCESS) {
      delete left;
      return rc;
    }
    expr = new ConjunctionExpr(conjunction_node->type, left, right);
  } else {
    expr = left;
  }
  return RC::SUCCESS;
}

RC ArithmeticExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                          const ArithmeticExprSqlNode *arithmetic_node, Expression *&expr) {
  Expression *left, *right;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, arithmetic_node->left, left);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  if (arithmetic_node->type != ArithmeticType::NEGATIVE) {
    rc = Expression::create(db, default_table, tables, arithmetic_node->right, right);
    if (rc != RC::SUCCESS) {
      delete left;
      return rc;
    }
    AttrType target_type = AttrTypeCompare(left->value_type(), right->value_type());
    if (target_type == UNDEFINED || (rc = CastExpr::create(target_type, left)) != RC::SUCCESS ||
        (rc = CastExpr::create(target_type, right)) != RC::SUCCESS) {
      LOG_ERROR("fail to do arithmetic operation on %s %s", attr_type_to_string(left->value_type()),
                attr_type_to_string(right->value_type()));
      delete left;
      delete right;
      return RC::INTERNAL;
    }
  } else {
    right = nullptr;
  }
  expr = new ArithmeticExpr(arithmetic_node->type, left, right);
  return RC::SUCCESS;
}

static void join_fields(set<Field> &a, set<Field> &b) { a.insert(b.begin(), b.end()); }

set<Field> FieldExpr::reference_fields() const { return {field_}; }
set<Field> ValueExpr::reference_fields() const { return {}; }
set<Field> CastExpr::reference_fields() const { return child_->reference_fields(); }
set<Field> ComparisonExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    a = right_->reference_fields();
  join_fields(a, b);
  return a;
}
set<Field> ConjunctionExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    a = right_->reference_fields();
  join_fields(a, b);
  return a;
}
set<Field> ArithmeticExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    a = right_->reference_fields();
  join_fields(a, b);
  return a;
}
