#include "sql/operator/physical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"
#include "sql/stmt/update_stmt.h"
class UpdatePhysicalOperator : public PhysicalOperator {
private:
  PhysicalOperatorType type() const { return PhysicalOperatorType::UPDATE; }

  virtual RC open(Trx *trx);
  virtual RC next(Tuple *env_tuple) { return RC::RECORD_EOF; }
  virtual RC close() { return RC::SUCCESS; }

  virtual Tuple *current_tuple() { return nullptr; }

  friend class PhysicalPlanGenerator;

private:
  RC insert(vector<char> &v, RID &rid);
  RC insert_all(vector<vector<char>> &v);
  RC remove_all(const vector<RID> &rids);
  RC update(vector<char> v, vector<Value> &values, RID &rid);

  void rollback();

private:
  std::vector<UpdateUnit> units_;
  Table *table_;
  std::vector<std::vector<char>> deleted_records_;
  std::vector<RID> inserted_records_;
  Trx *trx_;
};
