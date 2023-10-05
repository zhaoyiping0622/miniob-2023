#include "sql/operator/cached_physical_operator.h"
#include "common/rc.h"
#include "history.h"
#include "sql/operator/physical_operator.h"

RC CachedPhysicalOperator::open(Trx *trx) {
  if (children_.size() != 1) {
    LOG_ERROR("cached physical operator does not have one child physical operator");
  }
  child_ = children_[0].get();
  index_ = -1;
  if (!inited_) {
    opened_ = true;
    return child_->open(trx);
  }
  return RC::SUCCESS;
}

RC CachedPhysicalOperator::next(Tuple *env_tuple) {
  if (!inited_) {
    RC rc = init(env_tuple);
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

RC CachedPhysicalOperator::init(Tuple *env_tuple) {
  RC rc = RC::SUCCESS;
  while ((rc = child_->next(env_tuple)) == RC::SUCCESS) {
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
    Record record(sub_tuple->cell_num());
    for (int i = 0; i < sub_tuple->cell_num(); i++) {
      rc = sub_tuple->cell_at(i, record[i]);
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }
    records_.push_back(record);
  }
  if (rc == RC::RECORD_EOF)
    return RC::SUCCESS;
  return rc;
}
