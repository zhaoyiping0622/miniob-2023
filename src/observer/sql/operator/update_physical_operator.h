#include "sql/operator/physical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"
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
  RC update(vector<char> v, RID &rid);

private:
  Field update_field_;
  Value value_;
  Table *table_;
};