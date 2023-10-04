#pragma once

#include "sql/operator/logical_operator.h"

class SubQueryLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const override { return LogicalOperatorType::SUB_QUERY; }
};
