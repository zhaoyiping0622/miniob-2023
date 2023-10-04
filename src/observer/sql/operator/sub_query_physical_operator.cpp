#include "sql/operator/sub_query_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include <memory>
#include <utility>

SubQueryPhysicalOperator::SubQueryPhysicalOperator(std::unique_ptr<PhysicalOperator> main_op) {
  add_child(std::move(main_op));
}

RC SubQueryPhysicalOperator::add_sub_query(std::unique_ptr<PhysicalOperator> sub_query, std::string name) {
  add_child(std::move(sub_query));
  speces_.push_back(TupleCellSpec(name.c_str()));
  return RC::SUCCESS;
}

RC SubQueryPhysicalOperator::open(Trx *trx) {
  if (children_.size() < 2) {
    LOG_ERROR("sub query physical operator has no sub queries");
    return RC::INVALID_ARGUMENT;
  }
  for (int i = 0; i < children_.size(); i++) {
    RC rc = children_[i]->open(trx);
    if (rc != RC::SUCCESS)
      return rc;
  }
  list_tuple_.set_speces(speces_);
  return RC::SUCCESS;
}
RC SubQueryPhysicalOperator::next(Tuple *env_tuple) {
  env_.set_left(env_tuple);
  RC rc = RC::SUCCESS;
  rc = children_[0]->next(env_tuple);
  if (rc != RC::SUCCESS)
    return rc;
  env_.set_right(children_[0]->current_tuple());
  vector<Value> values(speces_.size());
  for (int i = 1; i < children_.size(); i++) {
    rc = children_[i]->next(&env_);
    if (rc != RC::SUCCESS)
      return rc;
    auto *tuple = children_[i]->current_tuple();
    rc = tuple->cell_at(0, values[i - 1]);
    if (rc != RC::SUCCESS)
      return rc;
  }
  list_tuple_.set_cells(values);
  result_.set_right(&env_);
  result_.set_left(&list_tuple_);
  return RC::SUCCESS;
}
RC SubQueryPhysicalOperator::close() {
  for (int i = 0; i < children_.size(); i++) {
    RC rc = children_[i]->close();
    if (rc != RC::SUCCESS)
      return rc;
  }
  return RC::SUCCESS;
}

Tuple *SubQueryPhysicalOperator::current_tuple() { return &result_; }
