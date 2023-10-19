#pragma once
#include "logical_operator.h"
#include "sql/expr/tuple.h"
#include "sql/stmt/select_stmt.h"

class CreateTableLogicalOperator : public LogicalOperator {
public:
  virtual ~CreateTableLogicalOperator() = default;

  virtual LogicalOperatorType type() const { return LogicalOperatorType::CREATE_TABLE; }
  const TupleSchema *schema() const { return schema_.get(); }
  const std::string &table_name() const { return table_name_; }
  const std::vector<AttrInfoSqlNode> &attr_infos() const { return attr_infos_; }

  friend class LogicalPlanGenerator;
  friend class PhysicalPlanGenerator;

private:
  std::shared_ptr<TupleSchema> schema_;
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  Db *db_;
  std::vector<FieldInfo> types_;
};
