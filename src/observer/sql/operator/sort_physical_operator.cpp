#include "sql/operator/sort_physical_operator.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"
#include <compare>
#include <utility>

RC SortPhysicalOperator::open(Trx *trx) {
  if (children_.size() != 1) {
    LOG_WARN("sort physical operator has %d children", children_.size());
    return RC::INTERNAL;
  }
  auto &child = children_[0];
  return child->open(trx);
}

RC SortPhysicalOperator::next(Tuple *env_tuple) {
  if (idx_ == -1) {
    init(env_tuple);
  }
  idx_++;
  if (idx_ == values_.size())
    return RC::RECORD_EOF;
  tuple_.set_cells(values_[idx_].second);
  return RC::SUCCESS;
}

RC SortPhysicalOperator::close() {
  idx_ = -1;
  return children_[0]->close();
}

RC SortPhysicalOperator::init(Tuple *env_tuple) {
  int num = schema_->cell_num();
  speces_.resize(num);
  for (int i = 0; i < num; i++) {
    speces_[i] = schema_->cell_at(i);
  }
  tuple_.set_speces(speces_);
  RC rc = read_all(env_tuple);
  if (rc != RC::SUCCESS)
    return rc;
  // sort
  std::sort(values_.begin(), values_.end(), [&](pair<Record, Record> &a, pair<Record, Record> &b) {
    for (int i = 0; i < orders_.size(); i++) {
      auto cmp = (a.first[i] <=> b.first[i]);
      if (cmp == strong_ordering::equal)
        continue;
      if (cmp == strong_ordering::less) {
        return orders_[i] == Order::ASC;
      }
      if (cmp == strong_ordering::greater) {
        return orders_[i] == Order::DESC;
      }
      LOG_ERROR("unknown ordering");
    }
    //  keep index order
    return &a < &b;
  });
  return RC::SUCCESS;
}

RC SortPhysicalOperator::read_all(Tuple *env_tuple) {
  RC rc = RC::SUCCESS;
  while ((rc = children_[0]->next(env_tuple)) == RC::SUCCESS) {
    auto *subtuple = children_[0]->current_tuple();
    Record record(schema_->cell_num());
    for (int i = 0; i < schema_->cell_num(); i++) {
      auto &spec = schema_->cell_at(i);
      rc = subtuple->find_cell(spec, record[i]);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to sort, cannot read all suboperator records");
        return rc;
      }
    }
    Record key(speces_.size());
    for (int i = 0; i < orders_.size(); i++) {
      rc = subtuple->find_cell(speces_[i], key[i]);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to sort, cannot read sort fields");
        return rc;
      }
    }
    values_.emplace_back(std::move(key), std::move(record));
  }
  if (rc != RC::RECORD_EOF) {
    return rc;
  }
  return RC::SUCCESS;
}

Tuple *SortPhysicalOperator::current_tuple() { return &tuple_; }
