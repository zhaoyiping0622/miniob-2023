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
  tuple_.set_cells(values_[idx_].ret_fields);
  tuple_.set_record_map(values_[idx_].record_map);
  return RC::SUCCESS;
}

RC SortPhysicalOperator::close() {
  idx_ = -1;
  return children_[0]->close();
}

RC SortPhysicalOperator::init(Tuple *env_tuple) {
  int num = schema_->cell_num();
  vector<TupleCellSpec> speces;
  speces.resize(num);
  for (int i = 0; i < num; i++) {
    speces[i] = schema_->cell_at(i);
  }
  tuple_.set_speces(speces);
  RC rc = read_all(env_tuple);
  if (rc != RC::SUCCESS)
    return rc;
  // sort
  std::sort(values_.begin(), values_.end(), [&](SortRecord &a, SortRecord &b) {
    for (int i = 0; i < orders_.size(); i++) {
      if (a.sort_fields[i].is_null() && b.sort_fields[i].is_null())
        continue;
      auto cmp = (a.sort_fields[i] <=> b.sort_fields[i]);
      if (b.sort_fields[i].is_null())
        cmp = strong_ordering::greater;
      else if (a.sort_fields[i].is_null())
        cmp = strong_ordering::less;

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
    Record key(sort_speces_.size());
    for (int i = 0; i < orders_.size(); i++) {
      rc = subtuple->find_cell(sort_speces_[i], key[i]);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to sort, cannot read sort fields");
        return rc;
      }
    }
    SortRecord sr;
    sr.sort_fields.swap(key);
    sr.ret_fields.swap(record);
    sr.record_map = subtuple->get_record_map();
    values_.emplace_back(std::move(sr));
  }
  if (rc != RC::RECORD_EOF) {
    return rc;
  }
  return RC::SUCCESS;
}

Tuple *SortPhysicalOperator::current_tuple() { return &tuple_; }
