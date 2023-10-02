#pragma once
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "storage/field/field.h"
#include <memory>
#include <vector>

struct OrderByUnit {
  std::unique_ptr<Expression> expr;
  Order order;
};
class OrderByStmt {
public:
  std::vector<OrderByUnit> &order_by_units() { return order_by_units_; }
  void add_order_by_unit(OrderByUnit &unit) { order_by_units_.push_back(std::move(unit)); }

private:
  std::vector<OrderByUnit> order_by_units_;
};
