#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"
class CachedPhysicalOperator : public PhysicalOperator {
public:
  CachedPhysicalOperator();
  virtual ~CachedPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::CACHED; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override { return &tuple_; }

  std::vector<std::vector<Value>> &get_records() {
    if (!inited_) {
      init();
      inited_ = true;
    }
    return records_;
  }

private:
  using Record = std::vector<Value>;
  int index_ = -1;
  std::vector<Record> records_;

  bool inited_ = false;
  RC init();

  ValueListTuple tuple_;
  PhysicalOperator *child_ = nullptr;
  bool opened_ = false;
};

RC cache_child_physical_operator(PhysicalOperator *oper);
