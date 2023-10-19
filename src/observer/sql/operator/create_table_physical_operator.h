#pragma once

#include "common/rc.h"
#include "sql/operator/physical_operator.h"
#include "sql/stmt/select_stmt.h"
class CreateTablePhysicalOperator : public PhysicalOperator {
public:
  CreateTablePhysicalOperator() = default;
  virtual ~CreateTablePhysicalOperator() = default;
  PhysicalOperatorType type() const override { return PhysicalOperatorType::CREATE_TABLE; }

  RC open(Trx *trx) override;
  RC next(Tuple *env_tuple) override { return RC::RECORD_EOF; }
  RC close() override { return RC::SUCCESS; }
  Tuple *current_tuple() override { return nullptr; }

  friend class PhysicalPlanGenerator;

private:
  Db *db_;
  std::shared_ptr<TupleSchema> schema_;
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  std::vector<FieldInfo> types_;
};
