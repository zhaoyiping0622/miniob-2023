#pragma once

#include "sql/operator/logical_operator.h"
#include <utility>

class SubQueryLogicalOperator : public LogicalOperator {
public:
  SubQueryLogicalOperator(std::unique_ptr<LogicalOperator> &get_operator) { add_child(std::move(get_operator)); }
  LogicalOperatorType type() const override { return LogicalOperatorType::SUB_QUERY; }
};
