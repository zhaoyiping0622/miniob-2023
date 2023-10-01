#pragma once

#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "storage/field/field.h"
#include <cwchar>
#include <memory>

class AggregationUnit {
public:
  AggregationUnit(std::string name, AggregationType type, Expression *expression)
      : name_(name), type_(type), expression_(expression) {
    schema_ = std::make_unique<TupleSchema>();
    schema_->append_cell(name.c_str());
    fields_ = expression_->reference_fields();
  }

  const std::string &name() const { return name_; }
  std::unique_ptr<Expression> &expression() { return expression_; }
  std::unique_ptr<TupleSchema> &schema() { return schema_; }
  AttrType value_type();
  std::set<Field> reference_fields() { return fields_; }

private:
  std::unique_ptr<Expression> expression_;
  AggregationType type_;
  std::unique_ptr<TupleSchema> schema_;
  std::string name_;
  std::set<Field> fields_;
};

class AggregationStmt {
public:
  std::set<Field> &group_fields() { return group_fields_; }
  const std::vector<std::unique_ptr<AggregationUnit>> &aggregation_units() const { return aggregation_units_; }
  bool has_aggregate() const { return aggregation_units_.size() || group_fields_.size(); }
  void add_aggregation_unit(AggregationUnit* unit);

private:
  std::vector<std::unique_ptr<AggregationUnit>> aggregation_units_;
  std::set<Field> group_fields_;
};
