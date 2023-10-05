#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
class SubQueryPhysicalOperator : public PhysicalOperator {
public:
  SubQueryPhysicalOperator(std::unique_ptr<PhysicalOperator> main_op);
  PhysicalOperatorType type() const override { return PhysicalOperatorType::SUB_QUERY; }

  RC open(Trx *trx) override;
  RC next(Tuple *env_tuple) override;
  RC close() override;
  RC add_sub_query(std::unique_ptr<PhysicalOperator> sub_query, std::string name);

  virtual Tuple *current_tuple() override;

private:
  JoinedTuple env_;
  std::unique_ptr<PhysicalOperator> main_;
  JoinedTuple result_;
  ValueListTuple list_tuple_;
  std::vector<TupleCellSpec> speces_;
  Trx *trx_;
};
