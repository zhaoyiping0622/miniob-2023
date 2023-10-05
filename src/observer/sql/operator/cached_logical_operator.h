#pragma once

#include "sql/operator/logical_operator.h"
#include <utility>
class CachedLogicalOperator : public LogicalOperator {
public:
  CachedLogicalOperator(std::unique_ptr<LogicalOperator> &sub_operator) { add_child(std::move(sub_operator)); }
  LogicalOperatorType type() const override { return LogicalOperatorType::CACHED; }
};
