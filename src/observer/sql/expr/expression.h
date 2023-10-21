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

#pragma once

#include <functional>
#include <memory>
#include <regex>
#include <string.h>
#include <string>
#include <unordered_map>
#include <utility>

#include "common/log/log.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/db/db.h"
#include "storage/field/field.h"

using ExprGenerator = std::function<RC(const ExprSqlNode *, Expression *&)>;

class Tuple;

/**
 * @defgroup Expression
 * @brief 表达式
 */

/**
 * @brief 表达式的抽象描述
 * @ingroup Expression
 * @details 在SQL的元素中，任何需要得出值的元素都可以使用表达式来描述
 * 比如获取某个字段的值、比较运算、类型转换
 * 当然还有一些当前没有实现的表达式，比如算术运算。
 *
 * 通常表达式的值，是在真实的算子运算过程中，拿到具体的tuple后
 * 才能计算出来真实的值。但是有些表达式可能就表示某一个固定的
 * 值，比如ValueExpr。
 */
class Expression {
public:
  Expression() = default;
  virtual ~Expression() = default;

  /**
   * @brief 根据具体的tuple，来计算当前表达式的值。tuple有可能是一个具体某个表的行数据
   */
  virtual RC get_value(const Tuple &tuple, Value &value) const = 0;

  /**
   * @brief 在没有实际运行的情况下，也就是无法获取tuple的情况下，尝试获取表达式的值
   * @details 有些表达式的值是固定的，比如ValueExpr，这种情况下可以直接获取值
   */
  virtual RC try_get_value(Value &value) const { return RC::UNIMPLENMENT; }

  /**
   * @brief 表达式的类型
   * 可以根据表达式类型来转换为具体的子类
   */
  virtual ExprType type() const = 0;

  /**
   * @brief 表达式值的类型
   * @details 一个表达式运算出结果后，只有一个值
   */
  virtual AttrType value_type() const = 0;

  /**
   * @brief 表达式的名字，比如是字段名称，或者用户在执行SQL语句时输入的内容
   */
  virtual std::string name() const { return name_; }
  virtual void set_name(std::string name) { name_ = name; }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  virtual std::set<Field> reference_fields() const = 0;

  virtual std::string to_string() const = 0;

private:
  std::string name_;
};

/**
 * @brief 字段表达式
 * @ingroup Expression
 */
class FieldExpr : public Expression {
public:
  FieldExpr() = default;
  FieldExpr(const Table *table, const FieldMeta *field) : field_(table, field) {
    spec_ = TupleCellSpec(table_name(), field_name());
  }
  FieldExpr(const Field &field) : field_(field) { spec_ = TupleCellSpec(table_name(), field_name()); }

  virtual ~FieldExpr() = default;

  ExprType type() const override { return ExprType::FIELD; }
  AttrType value_type() const override { return field_.attr_type(); }

  // std::string name() const override;

  Field &field() { return field_; }

  const Field &field() const { return field_; }

  const char *table_name() const { return field_.table_name(); }

  const char *field_name() const { return field_.field_name(); }

  RC get_value(const Tuple &tuple, Value &value) const override;

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const FieldExprSqlNode *field_node, Expression *&expr);

  static RC create(Table *table, const FieldMeta *field, std::string table_name, Expression *&expr);

  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override { return string(table_name()) + "." + field_name(); }

private:
  Field field_;
  TupleCellSpec spec_;
};

/**
 * @brief 常量值表达式
 * @ingroup Expression
 */
class ValueExpr : public Expression {
public:
  ValueExpr() = default;
  explicit ValueExpr(const Value &value) : value_(value) {}

  virtual ~ValueExpr() = default;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override {
    value = value_;
    return RC::SUCCESS;
  }

  ExprType type() const override { return ExprType::VALUE; }

  AttrType value_type() const override { return value_.attr_type(); }

  void get_value(Value &value) const { value = value_; }

  const Value &get_value() const { return value_; }

  static RC create(const ValueExprSqlNode *value_node, Expression *&expr);

  set<Field> reference_fields() const override;

  virtual std::string to_string() const override { return "Value(" + value_.to_string() + ")"; }

private:
  Value value_;
};

/**
 * @brief 类型转换表达式
 * @ingroup Expression
 */
class CastExpr : public Expression {
public:
  CastExpr(std::unique_ptr<Expression> child, AttrType cast_type);
  CastExpr(Expression *child, AttrType cast_type);
  virtual ~CastExpr();

  ExprType type() const override { return ExprType::CAST; }
  RC get_value(const Tuple &tuple, Value &value) const override;

  RC try_get_value(Value &value) const override;

  AttrType value_type() const override { return cast_type_; }

  std::unique_ptr<Expression> &child() { return child_; }

  static RC create(AttrType target_type, Expression *&expr);

  set<Field> reference_fields() const override;

  virtual std::string to_string() const override { return child_->to_string(); }

private:
  RC cast(const Value &value, Value &cast_value) const;

private:
  std::unique_ptr<Expression> child_; ///< 从这个表达式转换
  AttrType cast_type_;                ///< 想要转换成这个类型
};

/**
 * @brief 比较表达式
 * @ingroup Expression
 */
class ComparisonExpr : public Expression {
public:
  ComparisonExpr(CompOp comp, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  ComparisonExpr(CompOp comp, Expression *left, Expression *right);
  virtual ~ComparisonExpr();

  ExprType type() const override { return ExprType::COMPARISON; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  AttrType value_type() const override { return BOOLEANS; }

  CompOp comp() const { return comp_; }

  std::unique_ptr<Expression> &left() { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }

  /**
   * 尝试在没有tuple的情况下获取当前表达式的值
   * 在优化的时候，可能会使用到
   */
  RC try_get_value(Value &value) const override;

  /**
   * compare the two tuple cells
   * @param value the result of comparison
   */
  RC compare_value(const Value &left, const Value &right, bool &value) const;
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ComparisonExprSqlNode *comparison_node, Expression *&expr, ExprGenerator *fallback);

  set<Field> reference_fields() const override;

  virtual std::string to_string() const override {
    string ret;
    if (left_)
      ret += left_->to_string();
    ret += " cmpop ";
    if (right_)
      ret += right_->to_string();
    return "(" + ret + ")";
  }

private:
  CompOp comp_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

/**
 * @brief 联结表达式
 * @ingroup Expression
 * 多个表达式使用同一种关系(AND或OR)来联结
 * 当前miniob仅有AND操作
 */
class ConjunctionExpr : public Expression {
public:
  ConjunctionExpr(ConjunctionType type, Expression *left, Expression *right);
  ConjunctionExpr(ConjunctionType type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ConjunctionExpr() = default;

  ExprType type() const override { return ExprType::CONJUNCTION; }

  AttrType value_type() const override { return BOOLEANS; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  ConjunctionType conjunction_type() const { return conjunction_type_; }

  std::unique_ptr<Expression> &left() { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ConjunctionExprSqlNode *conjunction_node, Expression *&expr, ExprGenerator *fallback);

  set<Field> reference_fields() const override;

  virtual std::string to_string() const override {
    string ret;
    if (left_)
      ret += left_->to_string();
    ret += " conjop ";
    if (right_)
      ret += right_->to_string();
    return "(" + ret + ")";
  }

private:
  ConjunctionType conjunction_type_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

class ContainExpr : public Expression {
public:
  ContainExpr(ContainType type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
      : contain_type_(type), left_(std::move(left)), right_(std::move(right)) {}
  RC get_value(const Tuple &tuple, Value &value) const override;
  ExprType type() const override { return ExprType::CONTAIN; }
  AttrType value_type() const override { return BOOLEANS; }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ContainExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override;

private:
  ContainType contain_type_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

class ExistsExpr : public Expression {
public:
  ExistsExpr(bool exists, std::unique_ptr<Expression> left) : exists_(exists), left_(std::move(left)) {}
  RC get_value(const Tuple &tuple, Value &value) const override;
  ExprType type() const override { return ExprType::EXISTS; }
  AttrType value_type() const override { return BOOLEANS; }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ExistsExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override;

private:
  bool exists_;
  std::unique_ptr<Expression> left_;
};

class NullCheckExpr : public Expression {
public:
  NullCheckExpr(bool is_null, std::unique_ptr<Expression> left) : is_null_(is_null), left_(std::move(left)) {}
  RC get_value(const Tuple &tuple, Value &value) const override;
  ExprType type() const override { return ExprType::NULL_CHECK; }
  AttrType value_type() const override { return BOOLEANS; }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const NullCheckExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override;

private:
  bool is_null_;
  std::unique_ptr<Expression> left_;
};

class LikeExpr : public Expression {
public:
  LikeExpr(bool like, std::unique_ptr<Expression> left, std::string like_s)
      : like_(like), left_(std::move(left)), like_s_("^" + like_s.substr(1, like_s.size() - 2) + "$") {
    string pattern;
    for (auto x : like_s_) {
      if (x == '_') {
        pattern += "[^']";
      } else if (x == '%') {
        pattern += "[^']*";
      } else {
        pattern += x;
      }
    }
    regex_ = std::regex(pattern);
  }

  RC get_value(const Tuple &tuple, Value &value) const override;
  ExprType type() const override { return ExprType::LIKE; }
  AttrType value_type() const override { return BOOLEANS; }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const LikeExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override;

private:
  bool like_;
  std::unique_ptr<Expression> left_;
  std::string like_s_;
  std::regex regex_;
};

/**
 * @brief 算术表达式
 * @ingroup Expression
 */
class ArithmeticExpr : public Expression {
public:
  ArithmeticExpr(ArithmeticType type, Expression *left, Expression *right);
  ArithmeticExpr(ArithmeticType type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ArithmeticExpr() = default;

  ExprType type() const override { return ExprType::ARITHMETIC; }

  AttrType value_type() const override;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override;

  ArithmeticType arithmetic_type() const { return arithmetic_type_; }

  std::unique_ptr<Expression> &left() { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const ArithmeticExprSqlNode *arithmetic_node, Expression *&expr, ExprGenerator *fallback);

  set<Field> reference_fields() const override;

  virtual std::string to_string() const override {
    string ret;
    if (left_)
      ret += left_->to_string();
    ret += " arithmetic op ";
    if (right_)
      ret += right_->to_string();
    return "(" + ret + ")";
  }

private:
  RC calc_value(const Value &left_value, const Value &right_value, Value &value) const;

private:
  ArithmeticType arithmetic_type_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

/**
 * @brief 命名表达式
 * @ingroup Expression
 */
class NamedExpr : public Expression {
public:
  NamedExpr(AttrType value_type, TupleCellSpec spec, Table *table = nullptr);
  virtual ~NamedExpr() = default;

  virtual RC get_value(const Tuple &tuple, Value &value) const override;
  virtual RC try_get_value(Value &value) const override;
  virtual ExprType type() const override;
  virtual AttrType value_type() const override;
  virtual std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override { return name(); }

  Table *table() const { return table_; }
  TupleCellSpec &spec() { return spec_; }
  Field &field() { return field_; }

private:
  AttrType value_type_;
  TupleCellSpec spec_;
  Table *table_;
  Field field_;
};

class SetExpr : public Expression {
public:
  ExprType type() const override { return ExprType::SET; }
  AttrType value_type() const override { return LISTS; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const SetExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  std::set<Field> reference_fields() const override;

  std::string to_string() const override;

private:
  std::vector<std::unique_ptr<Expression>> children_;
  ValueListMap values_;
};

class SelectStmt;

class ListExpr : public NamedExpr {
public:
  ListExpr(SelectStmt *select, std::string name);

  ExprType type() const override { return ExprType::LIST; }
  SelectStmt *get_select() { return select_; }
  int get_column_num() { return column_num_; }

private:
  SelectStmt *select_;
  int column_num_;
};

class FunctionExpr : public Expression {
public:
  FunctionExpr(FunctionType type, std::vector<std::unique_ptr<Expression>> &children);
  FunctionType function_type() const { return function_type_; }
  ExprType type() const override { return ExprType::FUNCTION; }
  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override;
  AttrType value_type() const override;

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
                   const FunctionExprSqlNode *expr_node, Expression *&expr, ExprGenerator *fallback);

  RC calc_value(Value &out, std::vector<const Value *> &in) const;

  static RC check_function(FunctionType type, std::vector<AttrType> &attrs);
  std::set<Field> reference_fields() const override;

  virtual std::string to_string() const override { return "func(" + children_[0]->to_string() + ")"; }

private:
  FunctionType function_type_;
  std::vector<std::unique_ptr<Expression>> children_;
};
