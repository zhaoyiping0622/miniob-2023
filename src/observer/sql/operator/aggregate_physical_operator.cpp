#include "sql/operator/aggregate_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
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
    groupby_speces_.push_back(TupleCellSpec(x.table_name(), x.field_name()));
    project->add_projection(x);
    auto field_expr = unique_ptr<Expression>(new FieldExpr(x));
    project->add_expression(field_expr);
  }
  for (auto &x : aggregation_units_) {
    aggregation_speces_.push_back(TupleCellSpec(x->name().c_str()));
    auto &expression = x->expression();
    if (expression->type() == ExprType::FIELD) {
      FieldExpr &field_expr = static_cast<FieldExpr &>(*expression);
      project->add_projection(field_expr.field());
    } else {
      project->add_projection(x->expression()->name().c_str());
    }
    project->add_expression(x->expression());
  }
  children_.emplace_back(std::move(project));
  groupby_tuple_.set_speces(groupby_speces_);
  valuelist_tuple_.set_speces(aggregation_speces_);
}

AggregatePhysicalOperator::~AggregatePhysicalOperator() {}

RC AggregatePhysicalOperator::calculate_all(Tuple *env_tuple) {
  auto &child = children_[0];
  RC rc = RC::SUCCESS;
  while ((rc = child->next(env_tuple)) == RC::SUCCESS) {
    Tuple *tuple = child->current_tuple();
    vector<Value> record(groupby_speces_.size());
    vector<Value> values(aggregation_speces_.size());
    vector<Value> non_null_record;
    int bitmap = 0;
    for (int i = 0; i < record.size(); i++) {
      rc = tuple->cell_at(i, record[i]);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to read tuple cell idx=%d", i);
        return rc;
      }
      if (record[i].is_null()) {
        bitmap |= 1 << i;
      } else {
        non_null_record.push_back(record[i]);
      }
    }
    for (int i = 0; i < values.size(); i++) {
      rc = tuple->cell_at(i + record.size(), values[i]);
      if (rc != RC::SUCCESS) {
        LOG_WARN("fail to read tuple cell idx=%d", i);
        return rc;
      }
    }
    auto &map = map_[bitmap];
    auto it = map.find(non_null_record);
    if (it == map.end()) {
      vector<unique_ptr<Aggregator>> aggregators(values.size());
      for (int i = 0; i < values.size(); i++) {
        aggregators[i].reset(Aggregator::create(aggregation_units_[i]->aggregation_type(), values[i]));
      }
      map[non_null_record] = std::move(aggregators);
      records_.emplace_back(record, non_null_record, bitmap);
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

RC AggregatePhysicalOperator::next(Tuple *env_tuple) {
  RC rc = RC::SUCCESS;
  if (idx_ == -1) {
    rc = calculate_all(env_tuple);
    if (rc != RC::SUCCESS)
      return rc;
  }
  idx_++;
  if (idx_ == records_.size()) {
    return RC::RECORD_EOF;
  }
  auto &record = records_[idx_];
  int bitmap = record.bitmap;
  auto &nonnull = record.non_null_value;
  auto &map = map_[bitmap];
  auto it = map.find(nonnull);
  if (it == map.end()) {
    LOG_ERROR("map fail to find record");
    return RC::INTERNAL;
  }
  auto &aggregators = it->second;
  vector<Value> values(aggregators.size());
  for (int i = 0; i < values.size(); i++)
    values[i] = aggregators[i]->get_value();
  groupby_tuple_.set_cells(record.value);
  valuelist_tuple_.set_cells(values);
  return RC::SUCCESS;
}

RC AggregatePhysicalOperator::close() {
  map_.clear();
  records_.clear();
  idx_ = -1;
  valuelist_tuple_.set_cells(std::vector<Value>());
  groupby_tuple_.set_cells(std::vector<Value>());
  auto &child = children_[0];
  return child->close();
}

/////////////////////////////////////////////////////////////////////////////////
/// Aggregators
/////////////////////////////////////////////////////////////////////////////////

RC CountAggregator::add_value(Value value) {
  if (!value.is_null())
    count_++;
  return RC::SUCCESS;
}

Value CountAggregator::get_value() const { return Value(count_); }

RC MinAggregator::add_value(Value value) {
  if (value.is_null())
    return RC::SUCCESS;
  if (now_.is_null())
    now_ = value;
  else if (value.compare(now_) < 0)
    now_ = value;
  return RC::SUCCESS;
}
Value MinAggregator::get_value() const { return now_; }

RC MaxAggregator::add_value(Value value) {
  if (value.is_null())
    return RC::SUCCESS;
  if (now_.is_null())
    now_ = value;
  else if (value.compare(now_) > 0)
    now_ = value;
  return RC::SUCCESS;
}
Value MaxAggregator::get_value() const { return now_; }

RC SumAggregator::add_value(Value value) {
  if (value.is_null())
    return RC::SUCCESS;
  if (now_.is_null())
    now_ = value;
  else if (now_.attr_type() == FLOATS || value.attr_type() == FLOATS) {
    float sum = now_.get_float() + value.get_float();
    now_.set_float(sum);
  } else {
    now_.set_int(now_.get_int() + value.get_int());
  }
  return RC::SUCCESS;
}
Value SumAggregator::get_value() const { return now_; }

RC AvgAggregator::add_value(Value value) {
  if (value.is_null())
    return RC::SUCCESS;
  count_++;
  now_ += value.get_float();
  return RC::SUCCESS;
}
Value AvgAggregator::get_value() const {
  Value ret;
  if (count_)
    ret.set_float(now_ / count_);
  else
    ret.set_null();
  return ret;
}

Aggregator *Aggregator::create(AggregationType type, Value value) {
  switch (type) {
  case AggregationType::AGGR_COUNT: return new CountAggregator(value);
  case AggregationType::AGGR_MIN: return new MinAggregator(value);
  case AggregationType::AGGR_MAX: return new MaxAggregator(value);
  case AggregationType::AGGR_SUM: return new SumAggregator(value);
  case AggregationType::AGGR_AVG: return new AvgAggregator(value);
  }
  return nullptr;
}
