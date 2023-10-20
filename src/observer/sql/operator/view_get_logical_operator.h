#pragma once

#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"
class ViewGetLogicalOperator : public LogicalOperator {
public:
  virtual LogicalOperatorType type() const override { return LogicalOperatorType::VIEW_GET; }
  ViewGetLogicalOperator(View *view, std::vector<Field> fields, bool readonly);

private:
  View *view_;
  std::vector<Field> fields_;
  bool readonly_;
};
