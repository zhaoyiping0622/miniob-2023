#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
class RenamePhysicalOperator : public PhysicalOperator {
public:
  RenamePhysicalOperator(std::vector<std::pair<TupleCellSpec, TupleCellSpec>> spec_map);
  PhysicalOperatorType type() const override { return PhysicalOperatorType::RENAME; }

  virtual RC open(Trx *trx) override;
  virtual RC next(Tuple *env_tuple) override;
  virtual RC close() override;

  virtual Tuple *current_tuple() override;
  virtual std::string name() const override { return "RENAME"; }

private:
  RenameTuple tuple_;
};
