#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/orderby_stmt.h"
#include <memory>
#include <utility>
class SortLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const { return LogicalOperatorType::SORT; }

  void add_order_unit(OrderByUnit &&unit);
  std::shared_ptr<TupleSchema> &schema() { return schema_; }
  void set_schema(std::shared_ptr<TupleSchema> schema) { schema_ = std::move(schema); }
  std::vector<Order> &orders() { return orders_; }

private:
  std::vector<Order> orders_;
  std::shared_ptr<TupleSchema> schema_;
};
