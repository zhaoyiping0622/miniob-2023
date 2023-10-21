#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "storage/record/record.h"
#include <memory>
struct SortRecord {
  std::vector<Value> sort_fields;
  std::vector<Value> ret_fields;
  TableRecordMap record_map;
};
class SortPhysicalOperator : public PhysicalOperator {
public:
  using Record = std::vector<Value>;
  PhysicalOperatorType type() const override { return PhysicalOperatorType::SORT; }

  virtual RC open(Trx *trx) override;
  virtual RC next(Tuple *env_tuple) override;
  virtual RC close() override;

  virtual Tuple *current_tuple() override;

private:
  RC init(Tuple *env_tuple);
  RC read_all(Tuple *env_tuple);

  int idx_ = -1;
  std::vector<SortRecord> values_;

  std::vector<int> sort_indexes_;

  std::shared_ptr<TupleSchema> schema_;
  std::vector<TupleCellSpec> sort_speces_;
  std::vector<Order> orders_;
  ValueListTuple tuple_;
  friend class PhysicalPlanGenerator;
};
