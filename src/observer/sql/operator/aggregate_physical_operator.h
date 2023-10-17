#pragma once

#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/aggregation_stmt.h"
#include "storage/field/field.h"

class Aggregator;

class AggregatePhysicalOperator : public PhysicalOperator {
public:
  AggregatePhysicalOperator(std::set<Field> &group_fields,
                            std::vector<std::unique_ptr<AggregationUnit>> &aggregation_units,
                            std::unique_ptr<PhysicalOperator> &child);
  virtual ~AggregatePhysicalOperator();

  PhysicalOperatorType type() const override { return PhysicalOperatorType::AGGREGATE; }

  RC open(Trx *trx) override;
  RC next(Tuple *env_tuple) override;
  RC close() override;

  Tuple *current_tuple() override { return &joined_tuple_; }

private:
  std::set<Field> group_fields_;
  std::vector<std::unique_ptr<AggregationUnit>> aggregation_units_;

private:
  JoinedTuple joined_tuple_;
  ValueListTuple groupby_tuple_;
  ValueListTuple valuelist_tuple_;
  std::vector<TupleCellSpec> groupby_speces_;
  std::vector<TupleCellSpec> aggregation_speces_;

  struct RecordCache {
    std::vector<Value> value;
    std::vector<Value> non_null_value;
    int bitmap;
    RecordCache(std::vector<Value> value, std::vector<Value> non_null_value, int bitmap)
        : value(value), non_null_value(non_null_value), bitmap(bitmap) {}
  };

private:
  std::map<int, std::map<std::vector<Value>, std::vector<std::unique_ptr<Aggregator>>>> map_;
  std::vector<RecordCache> records_;
  int idx_ = -1;

  RC calculate_all(Tuple *env_tuple);
};

class Aggregator {
public:
  virtual RC add_value(Value value) = 0;
  virtual AggregationType type() const = 0;
  virtual Value get_value() const = 0;
  static Aggregator *create(AggregationType type, Value value);
};

class CountAggregator : public Aggregator {
public:
  CountAggregator(Value init_value) : count_(!init_value.is_null()) {}
  AggregationType type() const override { return AggregationType::AGGR_COUNT; }
  RC add_value(Value value) override;
  Value get_value() const override;

private:
  int count_;
};

class MinAggregator : public Aggregator {
public:
  MinAggregator(Value init_value) : now_(init_value) {}
  AggregationType type() const override { return AggregationType::AGGR_MIN; }
  RC add_value(Value value) override;
  Value get_value() const override;

private:
  Value now_;
};

class MaxAggregator : public Aggregator {
public:
  MaxAggregator(Value init_value) : now_(init_value) {}
  AggregationType type() const override { return AggregationType::AGGR_MAX; }
  RC add_value(Value value) override;
  Value get_value() const override;

private:
  Value now_;
};

class SumAggregator : public Aggregator {
public:
  SumAggregator(Value init_value) : now_(init_value) {}
  AggregationType type() const override { return AggregationType::AGGR_SUM; }
  RC add_value(Value value) override;
  Value get_value() const override;

private:
  Value now_;
};

class AvgAggregator : public Aggregator {
public:
  AvgAggregator(Value init_value)
      : now_(init_value.is_null() ? 0 : init_value.get_float()), count_(!init_value.is_null()) {}
  AggregationType type() const override { return AggregationType::AGGR_AVG; }
  RC add_value(Value value) override;
  Value get_value() const override;

private:
  double now_ = 0;
  int count_;
};
