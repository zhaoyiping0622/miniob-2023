#pragma once
#include "sql/operator/logical_operator.h"
class RenameLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const override { return LogicalOperatorType::RENAME; }
  void add_rename(TupleCellSpec from, TupleCellSpec to);

  friend class PhysicalPlanGenerator;

private:
  std::vector<std::pair<TupleCellSpec, TupleCellSpec>> spec_map_;
};
