#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"
class UpdateLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const { return LogicalOperatorType::UPDATE; }
  UpdateLogicalOperator(Table *table, Field field, Value value) : table_(table), update_field_(field), value_(value) {}

  Field &update_field() { return update_field_; }
  Value &value() { return value_; }
  Table *table() { return table_; }

private:
  Field update_field_;
  Value value_;
  Table *table_;
};
