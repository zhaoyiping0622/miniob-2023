#include "sql/operator/logical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/field/field.h"
class UpdateLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const { return LogicalOperatorType::UPDATE; }
  UpdateLogicalOperator(Table *table, std::vector<UpdateUnit> &units) : table_(table), units_(std::move(units)) {}

  std::vector<UpdateUnit> &units() { return units_; }
  Table *table() { return table_; }

private:
  std::vector<UpdateUnit> units_;
  Table *table_;
};
