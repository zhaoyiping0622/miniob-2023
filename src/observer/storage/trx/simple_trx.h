#include "storage/clog/clog.h"
#include "storage/trx/trx.h"
#include <cstdint>
#include <mutex>

class SimpleTrxKit : public TrxKit {
public:
  SimpleTrxKit() = default;
  virtual ~SimpleTrxKit();
  virtual RC init() override;

  virtual const std::vector<FieldMeta> *trx_fields() const override;
  virtual Trx *create_trx(CLogManager *log_manager) override;
  virtual Trx *create_trx(int32_t trx_id) override;
  virtual Trx *find_trx(int32_t trx_id) override;
  virtual void all_trxes(std::vector<Trx *> &trxes) override;

  virtual void destroy_trx(Trx *trx) override;

private:
  int max_trx_id_{0};
  std::mutex mutex_;
  std::map<int32_t, Trx *> trxs_;
};

struct SimpleTrxOperation {
public:
  Operation::Type type;
  Table *table;
  std::vector<char> v;
  RID rid;
};

class SimpleTrx : public Trx {
public:
  SimpleTrx() = default;
  ~SimpleTrx() = default;

  SimpleTrx(int32_t trx_id, CLogManager *log_manager = nullptr);

  virtual RC insert_record(Table *table, Record &record) override;
  virtual RC delete_record(Table *table, Record &record) override;

  virtual RC visit_record(Table *table, Record &record, bool readonly) override;

  virtual RC start_if_need() override;
  virtual RC commit() override;
  virtual RC rollback() override;

  virtual int32_t id() const override;

  virtual RC redo(Db *db, const CLogRecord &log_record) override;

  void add_oper(const SimpleTrxOperation &oper);

private:
  CLogManager *log_manager_;
  int32_t trx_id_;
  bool recovering_{false};
  bool started_{false};
  std::map<RID, SimpleTrxOperation> head_opers_;
  std::map<RID, SimpleTrxOperation> tail_opers_;
};
