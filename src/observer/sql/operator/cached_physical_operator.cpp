#include "sql/operator/cached_physical_operator.h"
#include "history.h"
#include "sql/operator/physical_operator.h"

CachedPhysicalOperator::CachedPhysicalOperator() {
  if (children_.size() != 1) {
    LOG_ERROR("cached physical operator does not have one child physical operator");
    child_ = children_[0].get();
  }
  index_ = -1;
}

RC CachedPhysicalOperator::open(Trx *trx) {
  index_ = -1;
  if (!inited_) {
    opened_ = true;
    return child_->open(trx);
  }
  return RC::SUCCESS;
}

RC CachedPhysicalOperator::next() {
  if (!inited_) {
    RC rc = init();
    if (rc != RC::SUCCESS) {
      return rc;
    }
    inited_ = true;
  }
  index_++;
  if (index_ == records_.size()) {
    return RC::RECORD_EOF;
  }
  tuple_.set_cells(records_[index_]);
  return RC::SUCCESS;
}

RC CachedPhysicalOperator::close() {
  if (opened_) {
    opened_ = false;
    return child_->close();
  }
  return RC::SUCCESS;
}

RC CachedPhysicalOperator::init() {
  RC rc = RC::SUCCESS;
  while ((rc = child_->next()) == RC::SUCCESS) {
    Tuple *sub_tuple = child_->current_tuple();
    if (tuple_.cell_num() != sub_tuple->cell_num()) {
      std::vector<TupleCellSpec> speces(sub_tuple->cell_num());
      for (int i = 0; i < speces.size(); i++) {
        RC rc = sub_tuple->spec_at(i, speces[i]);
        if (rc != RC::SUCCESS) {
          return RC::INTERNAL;
        }
      }
      tuple_.set_speces(speces);
    }
    Record record(tuple_.cell_num());
    for (int i = 0; i < tuple_.cell_num(); i++) {
      sub_tuple->cell_at(i, record[i]);
    }
    records_.push_back(record);
  }
  if (rc == RC::RECORD_EOF)
    return RC::SUCCESS;
  return rc;
}

RC cache_child_physical_operator(PhysicalOperator *oper) {
  for (auto &x : oper->children()) {
    if (x->type() == PhysicalOperatorType::CACHED)
      continue;
    auto *oper = new CachedPhysicalOperator();
    oper->add_child(unique_ptr<PhysicalOperator>(x.release()));
    x.reset(oper);
  }
  return RC::SUCCESS;
}
