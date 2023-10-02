#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include <memory>
class SortPhysicalOperator : public PhysicalOperator {
public:
  using Record = std::vector<Value>;
  PhysicalOperatorType type() const override { return PhysicalOperatorType::SORT; }

  virtual RC open(Trx *trx) override;
  virtual RC next() override;
  virtual RC close() override;

  virtual Tuple *current_tuple() override;

private:
  RC init();
  RC read_all();

  int idx_ = -1;
  std::vector<pair<Record, Record>> values_;

  std::shared_ptr<TupleSchema> schema_;
  std::vector<TupleCellSpec> speces_;
  std::vector<Order> orders_;
  ValueListTuple tuple_;
  friend class PhysicalPlanGenerator;
};
