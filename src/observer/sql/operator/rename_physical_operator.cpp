#include "rename_physical_operator.h"

RenamePhysicalOperator::RenamePhysicalOperator(std::vector<std::pair<TupleCellSpec, TupleCellSpec>> spec_map)
    : tuple_(spec_map) {}

RC RenamePhysicalOperator::open(Trx *trx) {
  if (children_.size() != 1) {
    LOG_WARN("rename physical operator should have one child");
    return RC::INTERNAL;
  }
  auto &child = children_[0];
  return child->open(trx);
}
RC RenamePhysicalOperator::next(Tuple *env_tuple) { return children_[0]->next(env_tuple); }
RC RenamePhysicalOperator::close() { return children_[0]->close(); }
Tuple *RenamePhysicalOperator::current_tuple() {
  auto *tmp = children_[0]->current_tuple();
  tuple_.set_sub_tuple(tmp);
  return &tuple_;
}
