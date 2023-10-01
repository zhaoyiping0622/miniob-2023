#include "sql/operator/aggregate_physical_operator.h"
#include "sql/expr/expression.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include <memory>
AggregatePhysicalOperator::AggregatePhysicalOperator(set<Field> &group_fields,
                                                     vector<unique_ptr<AggregationUnit>> &aggregation_units,
                                                     unique_ptr<PhysicalOperator> &child)
    : group_fields_(group_fields), aggregation_units_(std::move(aggregation_units)) {
  joined_tuple_.set_left(&groupby_tuple_);
  joined_tuple_.set_right(&valuelist_tuple_);
  ProjectPhysicalOperator *project = new ProjectPhysicalOperator;
  project->add_child(std::move(child));
  for (auto &x : group_fields_) {
    groupby_speces_.push_back(new TupleCellSpec(x.table_name(), x.field_name()));
    project->add_projection(x);
    auto field_expr = unique_ptr<Expression>(new FieldExpr(x));
    project->add_expression(field_expr);
  }
  for (auto &x : aggregation_units_) {
    aggregation_speces_.push_back(new TupleCellSpec(x->name().c_str()));
    project->add_projection(x->expression()->name().c_str());
    project->add_expression(x->expression());
  }
  children_.emplace_back(std::move(project));
  groupby_tuple_.set_speces(groupby_speces_);
  valuelist_tuple_.set_speces(aggregation_speces_);
}

AggregatePhysicalOperator::~AggregatePhysicalOperator() {
  for (auto x : groupby_speces_) {
    delete x;
  }
  for (auto x : aggregation_speces_) {
    delete x;
  }
}

RC AggregatePhysicalOperator::calculate_all() {
  auto &child = children_[0];
  RC rc = RC::SUCCESS;
  while ((rc = child->next()) == RC::SUCCESS) {
    Tuple *tuple = child->current_tuple();
    vector<Value> record(groupby_speces_.size());
    vector<Value> values(aggregation_speces_.size());
    for (int i = 0; i < record.size(); i++) {
      tuple->cell_at(i, record[i]);
    }
    for (int i = 0; i < values.size(); i++) {
      tuple->cell_at(i + record.size(), values[i]);
    }
    auto it = map_.find(record);
    if (it == map_.end()) {
      vector<unique_ptr<Aggregator>> aggregators(values.size());
      for (int i = 0; i < values.size(); i++) {
        aggregators[i].reset(Aggregator::create(aggregation_units_[i]->aggregation_type(), values[i]));
      }
      map_[record] = std::move(aggregators);
      records_.emplace_back(std::move(record));
    } else {
      auto &aggregators = it->second;
      for (int i = 0; i < values.size(); i++) {
        aggregators[i]->add_value(values[i]);
      }
    }
  }
  if (rc == RC::RECORD_EOF) {
    return RC::SUCCESS;
  }
  return rc;
}

RC AggregatePhysicalOperator::open(Trx *trx) {
  auto &child = children_[0];
  idx_ = -1;
  return child->open(trx);
}

RC AggregatePhysicalOperator::next() {
  if (idx_ == -1)
    calculate_all();
  idx_++;
  if (idx_ == records_.size()) {
    return RC::RECORD_EOF;
  }
  auto &record = records_[idx_];
  auto &aggregators = map_[record];
  vector<Value> values(aggregators.size());
  for (int i = 0; i < values.size(); i++)
    values[i] = aggregators[i]->get_value();
  groupby_tuple_.set_cells(record);
  valuelist_tuple_.set_cells(values);
  return RC::SUCCESS;
}

RC AggregatePhysicalOperator::close() {
  map_.clear();
  records_.resize(0);
  idx_ = -1;
  auto &child = children_[0];
  return child->close();
}

/////////////////////////////////////////////////////////////////////////////////
/// Aggregators
/////////////////////////////////////////////////////////////////////////////////

RC CountAggregator::add_value(Value value) {
  count_++;
  return RC::SUCCESS;
}

Value CountAggregator::get_value() const { return Value(count_); }

RC MinAggregator::add_value(Value value) {
  if (value.compare(now_) < 0)
    now_ = value;
  return RC::SUCCESS;
}
Value MinAggregator::get_value() const { return now_; }

RC MaxAggregator::add_value(Value value) {
  if (value.compare(now_) > 0)
    now_ = value;
  return RC::SUCCESS;
}
Value MaxAggregator::get_value() const { return now_; }

RC AvgAggregator::add_value(Value value) {
  count_++;
  now_.set_float(now_.get_float() + (value.get_float() - now_.get_float()) / count_);
  return RC::SUCCESS;
}
Value AvgAggregator::get_value() const { return now_; }

Aggregator *Aggregator::create(AggregationType type, Value value) {
  switch (type) {
  case AggregationType::AGGR_COUNT: return new CountAggregator(value);
  case AggregationType::AGGR_MIN: return new MinAggregator(value);
  case AggregationType::AGGR_MAX: return new MaxAggregator(value);
  case AggregationType::AGGR_AVG: return new AvgAggregator(value);
  }
}
