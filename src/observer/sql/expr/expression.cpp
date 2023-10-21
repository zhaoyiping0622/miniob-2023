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
#include "common/math/regex.h"
#include "common/rc.h"
#include "sql/expr/tuple.h"
#include "sql/parser/date.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/select_stmt.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <utility>

using namespace std;

static void join_fields(set<Field> &a, const set<Field> &b) { a.insert(b.begin(), b.end()); }

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const {
  auto *table = field_.table();
  // if (table->view()) {
  //   auto *view = table->view();
  //   auto *view_field_meta = view->view_meta().field(field_name());
  //   if (view_field_meta == nullptr)
  //     return RC::NOTFOUND;
  //   return tuple.find_cell(view_field_meta->get_tuple_spec(), value);
  // }
  return tuple.find_cell(spec_, value);
}

// string FieldExpr::name() const { return field_.meta()->name(); }

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

  if (value.attr_type() == NULLS) {
    cast_value.set_null();
    return rc;
  }

  Value ret = value;
  Value::convert(cast_value.attr_type(), cast_type_, ret);
  cast_value = ret;
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
  if (left.attr_type() == NULLS || right.attr_type() == NULLS) {
    result = false;
    return RC::SUCCESS;
  }
  int cmp_result = left.compare(right);
  if (cmp_result == INVALID_COMPARE) {
    LOG_WARN("unable to compare");
    return RC::INVALID_ARGUMENT;
  }
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

  if (left_value.attr_type() == NULLS || right_value.attr_type() == NULLS) {
    value.set_null();
    return rc;
  }

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
        value.set_null();
      } else {
        value.set_int(left_value.get_int() / right_value.get_int());
      }
    } else {
      if (right_value.get_float() > -EPSILON && right_value.get_float() < EPSILON) {
        value.set_null();
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
  if (arithmetic_type_ != ArithmeticType::NEGATIVE) {
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }
  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

NamedExpr::NamedExpr(AttrType value_type, TupleCellSpec spec, Table *table)
    : value_type_(value_type), spec_(spec), table_(table) {}
RC NamedExpr::get_value(const Tuple &tuple, Value &value) const { return tuple.find_cell(spec_, value); }
RC NamedExpr::try_get_value(Value &value) const { return RC::INVALID_ARGUMENT; }
ExprType NamedExpr::type() const { return ExprType::NAMED; }
AttrType NamedExpr::value_type() const { return value_type_; }
set<Field> NamedExpr::reference_fields() const {
  if (table_)
    return {field_};
  return {};
}
RC Expression::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                      const ExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
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
    rc = ComparisonExpr::create(db, default_table, tables, expr_node->get_comparison(), expr, fallback);
    break;
  case ExprType::CONJUNCTION:
    rc = ConjunctionExpr::create(db, default_table, tables, expr_node->get_conjunction(), expr, fallback);
    break;
  case ExprType::ARITHMETIC:
    rc = ArithmeticExpr::create(db, default_table, tables, expr_node->get_arithmetic(), expr, fallback);
    break;
  case ExprType::FUNCTION:
    rc = FunctionExpr::create(db, default_table, tables, expr_node->get_function(), expr, fallback);
    break;
  case ExprType::CONTAIN:
    rc = ContainExpr::create(db, default_table, tables, expr_node->get_contain(), expr, fallback);
    break;
  case ExprType::EXISTS:
    rc = ExistsExpr::create(db, default_table, tables, expr_node->get_exists(), expr, fallback);
    break;
  case ExprType::LIKE: rc = LikeExpr::create(db, default_table, tables, expr_node->get_like(), expr, fallback); break;
  case ExprType::NULL_CHECK:
    rc = NullCheckExpr::create(db, default_table, tables, expr_node->get_null(), expr, fallback);
    break;
  case ExprType::SET: rc = SetExpr::create(db, default_table, tables, expr_node->get_set(), expr, fallback); break;
  default:
    if (fallback) {
      rc = (*fallback)(expr_node, expr);
    } else {
      rc = RC::INTERNAL;
    }
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

////////////////////////////////////////////////////////////////////////////////

static float round(float a, int bits) {
  for (int i = 0; i < bits; i++)
    a *= 10;
  a += 0.5;
  a = floor(a);
  for (int i = 0; i < bits; i++)
    a /= 10;
  return a;
}
RC FunctionExpr::check_function(FunctionType type, std::vector<AttrType> &attrs) {
  switch (type) {
  case FunctionType::LENGTH:
    if (attrs.size() != 1 || attrs[0] != CHARS)
      return RC::INVALID_ARGUMENT;
    break;
  case FunctionType::ROUND:
    if (attrs.size() == 0 || attrs.size() > 2 || attrs[0] != FLOATS)
      return RC::INVALID_ARGUMENT;
    break;
  case FunctionType::DATE_FORMAT:
    if (attrs.size() != 2 || (attrs[0] != DATES && attrs[0] != CHARS) || attrs[1] != CHARS)
      return RC::INVALID_ARGUMENT;
    break;
  }
  return RC::SUCCESS;
}

RC FunctionExpr::calc_value(Value &out, vector<const Value *> &in) const {
  const Value *in1 = in[0];
  const Value *in2 = nullptr;
  if (in.size() > 1)
    in2 = in[1];
  switch (function_type_) {
  case FunctionType::LENGTH: {
    out.set_int(in1->length());
    return RC::SUCCESS;
  }
  case FunctionType::ROUND: {
    if (in2 != nullptr) {
      out.set_float(round(in1->get_float(), in2->get_int()));
    } else {
      out.set_float(round(in1->get_float(), 0));
    }
    return RC::SUCCESS;
  }
  case FunctionType::DATE_FORMAT: {
    auto date = in1->get_date();
    if (date == INVALID_DATE) {
      return RC::INVALID_ARGUMENT;
    }
    string str = Date::to_string(date, in2->get_string());
    out.set_string(str.c_str());
    return RC::SUCCESS;
  }
  }
  return RC::INTERNAL;
}

RC FunctionExpr::get_value(const Tuple &tuple, Value &value) const {
  vector<Value> in(children_.size());
  vector<const Value *> inp(children_.size());
  for (int i = 0; i < children_.size(); i++) {
    RC rc = children_[i]->get_value(tuple, in[i]);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    inp[i] = &in[i];
  }
  return calc_value(value, inp);
}

RC FunctionExpr::try_get_value(Value &value) const {
  vector<Value> in(children_.size());
  vector<const Value *> inp(children_.size());
  for (int i = 0; i < children_.size(); i++) {
    RC rc = children_[i]->try_get_value(in[i]);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    inp[i] = &in[i];
  }
  return calc_value(value, inp);
}

AttrType FunctionExpr::value_type() const {
  switch (function_type_) {
  case FunctionType::LENGTH: return INTS;
  case FunctionType::ROUND: return FLOATS;
  case FunctionType::DATE_FORMAT: return CHARS;
  }
  return UNDEFINED;
}

FunctionExpr::FunctionExpr(FunctionType type, std::vector<std::unique_ptr<Expression>> &children)
    : function_type_(type), children_(std::move(children)) {}

////////////////////////////////////////////////////////////////////////////////

ListExpr::ListExpr(SelectStmt *select, std::string name)
    : NamedExpr(LISTS, TupleCellSpec(name.c_str())), select_(select), column_num_(select->schema()->cell_num()) {}

////////////////////////////////////////////////////////////////////////////////
RC SetExpr::get_value(const Tuple &tuple, Value &value) const {
  auto values = values_;
  for (auto &child : children_) {
    Value value;
    RC rc = child->get_value(tuple, value);
    if (rc != RC::SUCCESS)
      return rc;
    values[value]++;
  }
  value.set_list(values);
  return RC::SUCCESS;
}
std::set<Field> SetExpr::reference_fields() const {
  std::set<Field> ret;
  for (auto &child : children_) {
    join_fields(ret, child->reference_fields());
  }
  return ret;
}
std::string SetExpr::to_string() const {
  std::vector<std::string> ss;
  for (auto x : values_)
    ss.push_back(x.first.to_string());
  for (auto &child : children_)
    ss.push_back(child->to_string());
  std::string ret;
  common::merge_string(ret, ",", ss);
  return ret;
}

RC SetExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const SetExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  std::vector<std::unique_ptr<Expression>> children;
  ValueListMap values;
  RC rc = RC::SUCCESS;
  Expression *tmp;
  for (auto &x : expr_node->expressions) {
    rc = Expression::create(db, default_table, tables, x, tmp, fallback);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (tmp->type() == ExprType::VALUE) {
      values[static_cast<ValueExpr *>(tmp)->get_value()]++;
      delete tmp;
    } else {
      children.emplace_back(tmp);
    }
  }
  SetExpr *set_expr = new SetExpr();
  set_expr->values_.swap(values);
  set_expr->children_.swap(children);
  expr = set_expr;
  return RC::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

RC ContainExpr::get_value(const Tuple &tuple, Value &value) const {
  Value right_value;
  Value left_value;
  RC rc = RC::SUCCESS;
  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  if (right_value.attr_type() == NULLS) {
    value.set_boolean(contain_type_ == ContainType::NOT_IN);
    return RC::SUCCESS;
  }
  if (right_value.is_null() && contain_type_ == ContainType::NOT_IN) {
    value.set_boolean(false);
  } else {
    auto list = right_value.get_list();
    ValueList tmp(left_value);
    if (list->count(tmp) == 0) {
      value.set_boolean(contain_type_ == ContainType::NOT_IN);
    } else {
      value.set_boolean(contain_type_ == ContainType::IN);
    }
  }
  return RC::SUCCESS;
}

std::set<Field> ContainExpr::reference_fields() const { return left_->reference_fields(); }

std::string ContainExpr::to_string() const {
  stringstream ss;
  ss << left_->to_string();
  if (contain_type_ == ContainType::IN) {
    ss << " in ";
  } else {
    ss << " not in ";
  }
  ss << right_->to_string();
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

RC ExistsExpr::get_value(const Tuple &tuple, Value &value) const {
  Value left_value;
  RC rc = RC::SUCCESS;
  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS)
    return rc;
  if (left_value.is_null() || left_value.attr_type() == LISTS && left_value.get_list()->empty()) {
    value.set_boolean(!exists_);
  } else {
    value.set_boolean(exists_);
  }
  return RC::SUCCESS;
}

std::set<Field> ExistsExpr::reference_fields() const { return left_->reference_fields(); }

std::string ExistsExpr::to_string() const {
  stringstream ss;
  if (exists_) {
    ss << "exists ";
  } else {
    ss << "not exists ";
  }
  ss << left_->to_string();
  return ss.str();
}

RC ExistsExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                      const ExistsExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  RC rc = RC::SUCCESS;
  Expression *left;
  rc = Expression::create(db, default_table, tables, expr_node->left, left, fallback);
  if (rc != RC::SUCCESS)
    return rc;
  expr = new ExistsExpr(expr_node->exists, unique_ptr<Expression>(left));
  return RC::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

RC LikeExpr::get_value(const Tuple &tuple, Value &value) const {
  Value left_value;
  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS)
    return rc;
  value.set_boolean(std::regex_search(left_value.get_string(), regex_) == like_);
  return RC::SUCCESS;
}

std::set<Field> LikeExpr::reference_fields() const { return left_->reference_fields(); }

std::string LikeExpr::to_string() const {
  stringstream ss;
  ss << left_->to_string();
  if (like_) {
    ss << " like ";
  } else {
    ss << " not like ";
  }
  ss << like_s_;
  return ss.str();
}

RC LikeExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                    const LikeExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  Expression *tmp;
  RC rc = Expression::create(db, default_table, tables, expr_node->left, tmp, fallback);
  if (rc != RC::SUCCESS)
    return rc;
  if (tmp->value_type() != CHARS) {
    rc = CastExpr::create(CHARS, tmp);
    if (rc != RC::SUCCESS) {
      delete tmp;
      return rc;
    }
  }
  expr = new LikeExpr(expr_node->like, unique_ptr<Expression>(tmp), expr_node->like_s);
  return RC::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

RC NullCheckExpr ::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;
  rc = left_->get_value(tuple, value);
  if (rc != RC::SUCCESS)
    return rc;
  value.set_boolean(value.is_null() == is_null_);
  return rc;
}

std::set<Field> NullCheckExpr::reference_fields() const { return left_->reference_fields(); }
std::string NullCheckExpr::to_string() const {
  auto ret = left_->to_string();
  if (is_null_)
    ret += " is null";
  else
    ret += " is not null";
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                       string &table_name, const string &field_name, Table *&table, const FieldMeta *&field) {
  if (common::is_blank(table_name.c_str())) {
    table = default_table;
    table_name = default_table->name();
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
RC FieldExpr::create(Table *table, const FieldMeta *field, std::string table_name, Expression *&expr) {
  if (table->name() != table_name) {
    auto named_expr = new NamedExpr(field->type(), TupleCellSpec(table_name.c_str(), field->name()), table);
    named_expr->field() = Field(table, field);
    expr = named_expr;
  } else {
    expr = new FieldExpr(table, field);
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
  return create(table, field, table_name, expr);
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
    if (!Value::convert(value.attr_type(), target_type, value)) {
      LOG_WARN("cannot cast value %s from %s to %s", value.to_string().c_str(), attr_type_to_string(value.attr_type()),
               attr_type_to_string(target_type));
      return RC::INTERNAL;
    }
    delete expr;
    expr = new ValueExpr(value);
    return RC::SUCCESS;
  }
  expr = new CastExpr(expr, target_type);
  return RC::SUCCESS;
}

RC ComparisonExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                          const ComparisonExprSqlNode *comparison_node, Expression *&expr, ExprGenerator *fallback) {
  RC rc = RC::SUCCESS;
  Expression *left, *right;
  rc = Expression::create(db, default_table, tables, comparison_node->left, left, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  rc = Expression::create(db, default_table, tables, comparison_node->right, right, fallback);
  if (rc != RC::SUCCESS) {
    delete left;
    return rc;
  }
  if (left->value_type() == NULLS || right->value_type() == NULLS) {
    expr = new ValueExpr(Value(false));
    delete left;
    delete right;
    return RC::SUCCESS;
  }
  AttrType target_type = AttrTypeCompare(left->value_type(), right->value_type());
  if (target_type == UNDEFINED && left->value_type() != LISTS && right->value_type() != LISTS ||
      (rc = CastExpr::create(target_type, left)) != RC::SUCCESS ||
      (rc = CastExpr::create(target_type, right)) != RC::SUCCESS) {
    delete left;
    delete right;
    return RC::INTERNAL;
  };
  expr = new ComparisonExpr(comparison_node->op, left, right);
  return RC::SUCCESS;
}

RC ConjunctionExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                           const ConjunctionExprSqlNode *conjunction_node, Expression *&expr, ExprGenerator *fallback) {
  Expression *left, *right;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, conjunction_node->left, left, fallback);
  if (rc != RC::SUCCESS || (rc = CastExpr::create(BOOLEANS, left)) != RC::SUCCESS) {
    return rc;
  }
  if (conjunction_node->type != ConjunctionType::SINGLE) {
    rc = Expression::create(db, default_table, tables, conjunction_node->right, right, fallback);
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
                          const ArithmeticExprSqlNode *arithmetic_node, Expression *&expr, ExprGenerator *fallback) {
  Expression *left, *right;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, arithmetic_node->left, left, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  if (arithmetic_node->type != ArithmeticType::NEGATIVE) {
    rc = Expression::create(db, default_table, tables, arithmetic_node->right, right, fallback);
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

pair<int, int> function_args(FunctionType type) {
  switch (type) {
  case FunctionType::LENGTH: return {1, 1};
  case FunctionType::ROUND: return {1, 2};
  case FunctionType::DATE_FORMAT: return {2, 2};
  }
  return {-1, -1};
}

RC FunctionExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                        const FunctionExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  auto &children = expr_node->children;
  auto type = expr_node->type;
  auto range = function_args(type);
  if (children.size() < range.first || children.size() > range.second) {
    LOG_WARN("function args size mismatch");
    return RC::INVALID_ARGUMENT;
  }
  RC rc = RC::SUCCESS;
  vector<unique_ptr<Expression>> children_expression;
  vector<AttrType> types;
  for (auto x : children) {
    Expression *expr = nullptr;
    rc = Expression::create(db, default_table, tables, x, expr, fallback);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    types.push_back(expr->value_type());
    children_expression.emplace_back(expr);
  }
  rc = check_function(type, types);
  if (rc != RC::SUCCESS) {
    LOG_WARN("function type mismatch");
    return rc;
  }
  expr = new FunctionExpr(type, children_expression);
  return RC::SUCCESS;
}

RC ContainExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                       const ContainExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  std::unique_ptr<Expression> left, right;
  Expression *tmp;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, expr_node->left, tmp, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  left.reset(tmp);
  rc = Expression::create(db, default_table, tables, expr_node->right, tmp, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  right.reset(tmp);
  if (tmp->value_type() != LISTS) {
    LOG_WARN("contain expr right attr type should be LISTS");
    return RC::INVALID_ARGUMENT;
  }
  expr = new ContainExpr(expr_node->type, std::move(left), std::move(right));
  return RC::SUCCESS;
}

RC NullCheckExpr::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                         const NullCheckExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  std::unique_ptr<Expression> left;
  Expression *tmp;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, expr_node->left, tmp, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  left.reset(tmp);
  expr = new NullCheckExpr(expr_node->is_null, std::move(left));
  return RC::SUCCESS;
}

static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                 const ContainExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback) {
  Expression *tmp;
  unique_ptr<Expression> left, right;
  RC rc = RC::SUCCESS;
  rc = Expression::create(db, default_table, tables, expr_node->left, tmp, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  left.reset(tmp);
  if (fallback == nullptr) {
    LOG_WARN("list expr should be processed in fallback");
    return RC::INTERNAL;
  }
  rc = Expression::create(db, default_table, tables, expr_node->right, tmp, fallback);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  right.reset(tmp);
  if (right->type() != ExprType::LIST) {
    LOG_WARN("contain right value is not list");
    return RC::IN_WRONG_TYPE;
  }
  ListExpr *list_expr = static_cast<ListExpr *>(right.get());
  if (list_expr->get_column_num() != 1) {
    LOG_WARN("contain right value has multiple column");
    return RC::IN_WRONG_TYPE;
  }
  expr = new ContainExpr(expr_node->type, std::move(left), std::move(right));
  return RC::SUCCESS;
}

set<Field> FieldExpr::reference_fields() const { return {field_}; }
set<Field> ValueExpr::reference_fields() const { return {}; }
set<Field> CastExpr::reference_fields() const { return child_->reference_fields(); }
set<Field> ComparisonExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    b = right_->reference_fields();
  join_fields(a, b);
  return a;
}
set<Field> ConjunctionExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    b = right_->reference_fields();
  join_fields(a, b);
  return a;
}
set<Field> ArithmeticExpr::reference_fields() const {
  set<Field> a, b;
  if (left_)
    a = left_->reference_fields();
  if (right_)
    b = right_->reference_fields();
  join_fields(a, b);
  return a;
}

set<Field> FunctionExpr::reference_fields() const {
  set<Field> ret;
  for (auto &x : children_) {
    set<Field> se = x->reference_fields();
    join_fields(ret, se);
  }
  return ret;
}
