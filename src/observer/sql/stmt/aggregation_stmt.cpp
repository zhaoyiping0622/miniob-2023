#include "sql/stmt/aggregation_stmt.h"
#include "sql/parser/value.h"

AttrType AggregationUnit::value_type() {
  AttrType expr_type = expression_->value_type();
  switch (type_) {
  case AggregationType::AGGR_COUNT: return INTS;
  case AggregationType::AGGR_MIN: return expr_type;
  case AggregationType::AGGR_MAX: return expr_type;
  case AggregationType::AGGR_AVG: return FLOATS;
  case AggregationType::AGGR_SUM: {
    if (expr_type == INTS)
      return INTS;
    if (expr_type == FLOATS)
      return FLOATS;
    return UNDEFINED;
  }
  }
  return UNDEFINED;
}

void AggregationStmt::add_aggregation_unit(AggregationUnit *unit) { aggregation_units_.emplace_back(unit); }
