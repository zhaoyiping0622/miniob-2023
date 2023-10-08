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
// Created by Wangyunlai on 2021/5/14.
//

#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/record/record.h"

class Table;

/**
 * @defgroup Tuple
 * @brief Tuple 元组，表示一行数据，当前返回客户端时使用
 * @details 
 * tuple是一种可以嵌套的数据结构。
 * 比如select t1.a+t2.b from t1, t2;
 * 需要使用下面的结构表示：
 * @code {.cpp}
 *  Project(t1.a+t2.b)
 *        |
 *      Joined
 *      /     \
 *   Row(t1) Row(t2)
 * @endcode
 * 
 */

/**
 * @brief 元组的结构，包含哪些字段(这里成为Cell)，每个字段的说明
 * @ingroup Tuple
 */
class TupleSchema {
public:
  void append_cell(const TupleCellSpec &cell) { cells_.push_back(cell); }
  void append_cell(const char *table, const char *field) { append_cell(TupleCellSpec(table, field)); }
  void append_cell(const char *alias) { append_cell(TupleCellSpec(alias)); }
  int cell_num() const { return static_cast<int>(cells_.size()); }
  const TupleCellSpec &cell_at(int i) const { return cells_[i]; }

private:
  std::vector<TupleCellSpec> cells_;
};

/**
 * @brief 元组的抽象描述
 * @ingroup Tuple
 */
class Tuple {
public:
  Tuple() = default;
  virtual ~Tuple() = default;

  /**
   * @brief 获取元组中的Cell的个数
   * @details 个数应该与tuple_schema一致
   */
  virtual int cell_num() const = 0;

  /**
   * @brief 获取指定位置的Cell
   * 
   * @param index 位置
   * @param[out] cell  返回的Cell
   */
  virtual RC cell_at(int index, Value &cell) const = 0;

  /**
   * @brief 根据cell的描述，获取cell的值
   * 
   * @param spec cell的描述
   * @param[out] cell 返回的cell
   */
  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const = 0;

  virtual RC spec_at(int index, TupleCellSpec &spec) const = 0;

  virtual RC get_record(Table *table, Record *&record) = 0;

  virtual std::string to_string() const {
    std::string str;
    const int cell_num = this->cell_num();
    for (int i = 0; i < cell_num - 1; i++) {
      Value cell;
      cell_at(i, cell);
      str += cell.to_string();
      str += ", ";
    }

    if (cell_num > 0) {
      Value cell;
      cell_at(cell_num - 1, cell);
      str += cell.to_string();
    }
    return str;
  }
};

/**
 * @brief 一行数据的元组
 * @ingroup Tuple
 * @details 直接就是获取表中的一条记录
 */
class RowTuple : public Tuple {
public:
  RowTuple() = default;
  virtual ~RowTuple() {
    for (FieldExpr *spec : speces_) {
      delete spec;
    }
    speces_.clear();
  }

  void set_record(Record *record) { this->record_ = record; }

  void set_schema(const Table *table, const std::vector<FieldMeta> *fields) {
    table_ = table;
    this->speces_.reserve(fields->size());
    for (const FieldMeta &field : *fields) {
      speces_.push_back(new FieldExpr(table, &field));
    }
  }

  int cell_num() const override { return speces_.size(); }

  RC cell_at(int index, Value &cell) const override {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }

    FieldExpr *field_expr = speces_[index];
    const FieldMeta *field_meta = field_expr->field().meta();
    const FieldMeta *null_meta = table_->table_meta().null_field_meta();
    int null_flag = *(int *)(this->record_->data() + null_meta->offset());
    if (null_flag & (1 << field_meta->index())) {
      cell.set_null();
    } else if (field_meta->type() == TEXTS) {
      int offset = *(int *)(this->record_->data() + field_meta->offset());
      RC rc = const_cast<Table *>(table_)->get_text(offset, cell);
      if (rc != RC::SUCCESS) {
        return rc;
      }
    } else {
      cell.set_type(field_meta->type());
      cell.set_data(this->record_->data() + field_meta->offset(), field_meta->len());
    }
    return RC::SUCCESS;
  }

  RC spec_at(int index, TupleCellSpec &spec) const override {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }
    auto field_expr = speces_[index];
    spec = TupleCellSpec(field_expr->table_name(), field_expr->field_name());
    return RC::SUCCESS;
  }

  RC get_record(Table *table, Record *&record) override {
    if (strcmp(table->name(), table_->name()) == 0) {
      record = record_;
      return RC::SUCCESS;
    }
    return RC::NOTFOUND;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    const char *table_name = spec.table_name();
    const char *field_name = spec.field_name();
    if (0 != strcmp(table_name, table_->name())) {
      return RC::NOTFOUND;
    }

    for (size_t i = 0; i < speces_.size(); ++i) {
      const FieldExpr *field_expr = speces_[i];
      const Field &field = field_expr->field();
      if (0 == strcmp(field_name, field.field_name())) {
        return cell_at(i, cell);
      }
    }
    return RC::NOTFOUND;
  }

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif

  Record &record() { return *record_; }

  const Record &record() const { return *record_; }

private:
  Record *record_ = nullptr;
  const Table *table_ = nullptr;
  std::vector<FieldExpr *> speces_;
};

/**
 * @brief 从一行数据中，选择部分字段组成的元组，也就是投影操作
 * @ingroup Tuple
 * @details 一般在select语句中使用。
 * 投影也可以是很复杂的操作，比如某些字段需要做类型转换、重命名、表达式运算、函数计算等。
 * 当前的实现是比较简单的，只是选择部分字段，不做任何其他操作。
 */
class ProjectTuple : public Tuple {
public:
  ProjectTuple() = default;
  virtual ~ProjectTuple() {
    for (TupleCellSpec *spec : speces_) {
      delete spec;
    }
    speces_.clear();
  }

  void set_tuple(Tuple *tuple) { this->tuple_ = tuple; }

  void add_cell_spec(TupleCellSpec *spec) { speces_.push_back(spec); }
  void add_expression(std::unique_ptr<Expression> &expression) { expressions_.push_back(std::move(expression)); }
  int cell_num() const override { return speces_.size(); }

  RC cell_at(int index, Value &cell) const override {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::INTERNAL;
    }
    if (tuple_ == nullptr) {
      return RC::INTERNAL;
    }

    return expressions_[index]->get_value(*tuple_, cell);
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    for (int i = 0; i < expressions_.size(); i++) {
      if (*speces_[i] == spec) {
        return expressions_[i]->get_value(*tuple_, cell);
      }
    }
    return RC::NOTFOUND;
  }

  RC spec_at(int index, TupleCellSpec &spec) const override {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::NOTFOUND;
    }
    spec = *speces_[index];
    return RC::SUCCESS;
  }

  RC get_record(Table *table, Record *&record) override {
    if (tuple_)
      return tuple_->get_record(table, record);
    return RC::NOTFOUND;
  }

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::NOTFOUND;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif
private:
  std::vector<TupleCellSpec *> speces_;
  std::vector<std::unique_ptr<Expression>> expressions_;
  Tuple *tuple_ = nullptr;
};

class ExpressionTuple : public Tuple {
public:
  ExpressionTuple(std::vector<std::unique_ptr<Expression>> &expressions) : expressions_(expressions) {}

  virtual ~ExpressionTuple() {}

  int cell_num() const override { return expressions_.size(); }

  RC cell_at(int index, Value &cell) const override {
    if (index < 0 || index >= static_cast<int>(expressions_.size())) {
      return RC::INTERNAL;
    }

    const Expression *expr = expressions_[index].get();
    return expr->try_get_value(cell);
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    for (const std::unique_ptr<Expression> &expr : expressions_) {
      if (0 == strcmp(spec.alias(), expr->name().c_str())) {
        return expr->try_get_value(cell);
      }
    }
    return RC::NOTFOUND;
  }
  RC spec_at(int index, TupleCellSpec &spec) const override {
    if (index < 0 || index >= static_cast<int>(expressions_.size())) {
      return RC::NOTFOUND;
    }
    spec = TupleCellSpec(expressions_[index]->name().c_str());
    return RC::SUCCESS;
  }
  RC get_record(Table *table, Record *&record) override { return RC::NOTFOUND; }

private:
  const std::vector<std::unique_ptr<Expression>> &expressions_;
};

/**
 * @brief 一些常量值组成的Tuple
 * @ingroup Tuple
 */
class ValueListTuple : public Tuple {
public:
  ValueListTuple() = default;
  virtual ~ValueListTuple() = default;

  void set_cells(const std::vector<Value> &cells) { cells_ = cells; }
  void set_speces(const std::vector<TupleCellSpec> &speces) { speces_ = speces; }

  virtual int cell_num() const override { return static_cast<int>(std::max(speces_.size(), cells_.size())); }

  virtual RC cell_at(int index, Value &cell) const override {
    if (index < 0 || index >= cell_num()) {
      return RC::NOTFOUND;
    }

    cell = cells_[index];
    return RC::SUCCESS;
  }

  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    if (cells_.size() != speces_.size())
      return RC::INTERNAL;
    for (int i = 0; i < cells_.size(); i++)
      if (spec == speces_[i]) {
        cell = cells_[i];
        return RC::SUCCESS;
      }
    return RC::NOTFOUND;
  }

  RC spec_at(int index, TupleCellSpec &spec) const override {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::NOTFOUND;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }

  RC get_record(Table *table, Record *&record) override { return RC::NOTFOUND; }

private:
  std::vector<Value> cells_;
  std::vector<TupleCellSpec> speces_;
};

/**
 * @brief 将两个tuple合并为一个tuple
 * @ingroup Tuple
 * @details 在join算子中使用
 */
class JoinedTuple : public Tuple {
public:
  JoinedTuple() = default;
  virtual ~JoinedTuple() = default;

  void set_left(Tuple *left) { left_ = left; }
  void set_right(Tuple *right) { right_ = right; }

  int cell_num() const override { return (left_ ? left_->cell_num() : 0) + (right_ ? right_->cell_num() : 0); }

  RC cell_at(int index, Value &value) const override {
    const int left_cell_num = (left_ ? left_->cell_num() : 0);
    if (index > 0 && index < left_cell_num) {
      return left_->cell_at(index, value);
    }

    const int right_cell_num = (right_ ? right_->cell_num() : 0);
    if (index >= left_cell_num && index < left_cell_num + right_cell_num) {
      return right_->cell_at(index - left_cell_num, value);
    }

    return RC::NOTFOUND;
  }

  RC find_cell(const TupleCellSpec &spec, Value &value) const override {
    RC rc = (left_ ? left_->find_cell(spec, value) : RC::NOTFOUND);
    if (rc == RC::SUCCESS || rc != RC::NOTFOUND) {
      return rc;
    }

    return (right_ ? right_->find_cell(spec, value) : RC::NOTFOUND);
  }

  RC spec_at(int index, TupleCellSpec &spec) const override {
    const int left_cell_num = (left_ ? left_->cell_num() : 0);
    const int right_cell_num = (right_ ? right_->cell_num() : 0);
    int num = left_cell_num + right_cell_num;
    if (index < 0 || index >= num) {
      return RC::NOTFOUND;
    }
    if (index < left_cell_num) {
      return left_->spec_at(index, spec);
    }
    if (index >= left_cell_num)
      return right_->spec_at(index - left_cell_num, spec);
    return RC::NOTFOUND;
  }

  RC get_record(Table *table, Record *&record) override {
    RC rc = RC::NOTFOUND;
    if (left_) {
      rc = left_->get_record(table, record);
      if (rc == RC::SUCCESS)
        return rc;
    }
    if (right_) {
      rc = right_->get_record(table, record);
      if (rc == RC::SUCCESS)
        return rc;
    }
    return rc;
  }

private:
  Tuple *left_ = nullptr;
  Tuple *right_ = nullptr;
};
