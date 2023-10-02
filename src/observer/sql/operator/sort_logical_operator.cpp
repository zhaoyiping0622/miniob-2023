#include "sql/operator/sort_logical_operator.h"
#include "sql/stmt/orderby_stmt.h"

void SortLogicalOperator::add_order_unit(OrderByUnit &&unit) {
  expressions_.emplace_back(unit.expr.release());
  orders_.push_back(unit.order);
}
