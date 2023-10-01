#pragma once

#include "sql/operator/logical_operator.h"
#include "sql/stmt/aggregation_stmt.h"
#include "storage/field/field.h"

class AggregateLogicalOperator : public LogicalOperator {
public:
  AggregateLogicalOperator(AggregationStmt *stmt)
      : group_fields_(std::move(stmt->group_fields())), aggregation_units_(std::move(stmt->aggregation_units())) {}
  std::set<Field> &group_fields() { return group_fields_; }
  std::vector<std::unique_ptr<AggregationUnit>> &aggregation_units() { return aggregation_units_; }

  LogicalOperatorType type() const override { return LogicalOperatorType::AGGREGATE; }
  virtual ~AggregateLogicalOperator() = default;

private:
  std::set<Field> group_fields_;
  std::vector<std::unique_ptr<AggregationUnit>> aggregation_units_;
};
