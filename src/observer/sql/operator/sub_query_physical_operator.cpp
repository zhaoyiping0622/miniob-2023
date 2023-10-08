#include "sql/operator/sub_query_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/value.h"
#include <memory>
#include <utility>

SubQueryPhysicalOperator::SubQueryPhysicalOperator(std::unique_ptr<PhysicalOperator> main_op)
    : main_(std::move(main_op)) {}

RC SubQueryPhysicalOperator::add_sub_query(std::unique_ptr<PhysicalOperator> sub_query, std::string name) {
  add_child(std::move(sub_query));
  speces_.push_back(TupleCellSpec(name.c_str()));
  return RC::SUCCESS;
}

RC SubQueryPhysicalOperator::open(Trx *trx) {
  if (children_.size() < 1) {
    LOG_ERROR("sub query physical operator has no sub queries");
    return RC::INVALID_ARGUMENT;
  }
  RC rc = main_->open(trx);
  if (rc != RC::SUCCESS)
    return rc;
  list_tuple_.set_speces(speces_);
  trx_ = trx;
  return RC::SUCCESS;
}
RC SubQueryPhysicalOperator::next(Tuple *env_tuple) {
  RC rc = RC::SUCCESS;
  rc = main_->next(env_tuple);
  if (rc != RC::SUCCESS)
    return rc;
  env_.set_left(main_->current_tuple());
  env_.set_right(env_tuple);
  vector<Value> values(speces_.size());
  for (int i = 0; i < children_.size(); i++) {
    auto &child = children_[i];
    rc = child->open(trx_);
    if (rc != RC::SUCCESS)
      return rc;
    ValueListMap records;
    while ((rc = child->next(&env_)) == RC::SUCCESS) {
      Tuple *sub_tuple = child->current_tuple();
      Value tmp;
      rc = sub_tuple->cell_at(0, tmp);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      records[tmp]++;
    }
    if (rc != RC::RECORD_EOF) {
      return rc;
    }
    if (records.size()) {
      values[i].set_list(records);
    } else {
      values[i].set_null();
    }
    rc = child->close();
    if (rc != RC::SUCCESS) {
      return rc;
    }
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
