#include "storage/trx/simple_trx.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "storage/db/db.h"
#include "storage/trx/trx.h"
#include <cassert>
#include <mutex>

RC SimpleTrxKit::init() { return RC::SUCCESS; }

const std::vector<FieldMeta> *SimpleTrxKit::trx_fields() const { return nullptr; }

Trx *SimpleTrxKit::create_trx(CLogManager *log_manager) {
  std::unique_lock<std::mutex> lock(mutex_);
  max_trx_id_++;
  return trxs_[max_trx_id_] = new SimpleTrx(max_trx_id_, log_manager);
}

Trx *SimpleTrxKit::create_trx(int32_t trx_id) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (max_trx_id_ < trx_id)
    max_trx_id_ = trx_id;
  return trxs_[trx_id] = new SimpleTrx(trx_id);
}

Trx *SimpleTrxKit::find_trx(int32_t trx_id) {
  std::unique_lock<std::mutex> lock(mutex_);
  auto it = trxs_.find(trx_id);
  if (it != trxs_.end())
    return it->second;
  return nullptr;
}

void SimpleTrxKit::all_trxes(std::vector<Trx *> &trxes) {
  std::unique_lock<std::mutex> lock(mutex_);
  for (auto &x : trxs_) {
    trxes.push_back(x.second);
  }
}

void SimpleTrxKit::destroy_trx(Trx *trx) {
  std::unique_lock<std::mutex> lock(mutex_);
  auto it = trxs_.find(trx->id());
  if (it != trxs_.end()) {
    delete trx;
    trxs_.erase(it);
  }
}

SimpleTrxKit::~SimpleTrxKit() {
  for (auto &trx : trxs_) {
    delete trx.second;
  }
}

SimpleTrx::SimpleTrx(int32_t trx_id, CLogManager *log_manager)
    : log_manager_(log_manager), trx_id_(trx_id), recovering_(log_manager_ == nullptr),
      started_(log_manager_ == nullptr) {}

RC SimpleTrx::insert_record(Table *table, Record &record) {
  RC rc = table->insert_record(record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to insert record into table. rc=%s", strrc(rc));
    return rc;
  }

  if (!recovering_) {
    rc = log_manager_->append_log(CLogType::INSERT, trx_id_, table->table_id(), record.rid(), record.len(),
                                  0 /*offset*/, record.data());
    ASSERT(rc == RC::SUCCESS,
           "failed to append insert record log. trx id=%d, table id=%d, rid=%s, record len=%d, rc=%s", trx_id_,
           table->table_id(), record.rid().to_string().c_str(), record.len(), strrc(rc));
  }

  LOG_INFO("insert record log rid=%s", record.rid().to_string().c_str());

  SimpleTrxOperation oper;
  oper.type = Operation::Type::INSERT;
  oper.rid = record.rid();
  oper.table = table;
  add_oper(oper);

  return rc;
}

RC SimpleTrx::delete_record(Table *table, Record &record) {
  SimpleTrxOperation oper;
  oper.type = Operation::Type::DELETE;
  oper.table = table;
  oper.v = vector<char>(record.data(), record.data() + record.len());
  oper.rid = record.rid();

  RC rc = table->delete_record(record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to insert record into table. rc=%s", strrc(rc));
    return rc;
  }

  if (!recovering_) {
    rc = log_manager_->append_log(CLogType::DELETE, trx_id_, table->table_id(), oper.rid, oper.v.size(), 0 /*offset*/,
                                  oper.v.data());
    ASSERT(rc == RC::SUCCESS,
           "failed to append insert record log. trx id=%d, table id=%d, rid=%s, record len=%d, rc=%s", trx_id_,
           table->table_id(), oper.rid.to_string().c_str(), oper.v.size(), strrc(rc));
  }

  LOG_INFO("delete record log rid=%s", record.rid().to_string().c_str());

  add_oper(oper);

  return rc;
}

RC SimpleTrx::visit_record(Table *table, Record &record, bool readonly) { return RC::SUCCESS; }

RC SimpleTrx::start_if_need() {
  if (!started_) {
    started_ = true;
    log_manager_->begin_trx(trx_id_);
  }
  return RC::SUCCESS;
}

RC SimpleTrx::commit() {
  tail_opers_.clear();
  head_opers_.clear();
  if (!recovering_) {
    RC rc = log_manager_->commit_trx(trx_id_, trx_id_);
    return rc;
  }
  return RC::SUCCESS;
}

RC SimpleTrx::rollback() {
  RC rc = RC::SUCCESS;
  for (auto &[_, oper] : tail_opers_) {
    auto *table = oper.table;
    if (oper.type == Operation::Type::INSERT) {
      LOG_INFO("rollback delete_record rid=%s", oper.rid.to_string().c_str());
      rc = table->delete_record(oper.rid);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("failed to rollback, error in delete_record, rc=%s", strrc(rc));
      }
    }
  }
  for (auto &[_, oper] : head_opers_) {
    auto *table = oper.table;
    if (oper.type == Operation::Type::DELETE) {
      Record record;
      record.set_data(const_cast<char *>(oper.v.data()), oper.v.size());
      LOG_INFO("rollback recover_insert_record rid=%s", record.rid().to_string().c_str());
      rc = table->insert_record(record);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("failed to rollback, error in insert_record, rc=%s", strrc(rc));
      }
    }
  }
  tail_opers_.clear();
  head_opers_.clear();
  if (!recovering_) {
    log_manager_->rollback_trx(trx_id_);
  }
  return RC::SUCCESS;
}

int32_t SimpleTrx::id() const { return trx_id_; }

RC SimpleTrx::redo(Db *db, const CLogRecord &log_record) {
  Table *table = nullptr;
  auto &data_record = log_record.data_record();
  table = db->find_table(data_record.table_id_);
  if (table == nullptr && (log_record.log_type() == CLogType::INSERT || log_record.log_type() == CLogType::DELETE)) {
    LOG_WARN("table id=%d not found", data_record.table_id_);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  RC rc = RC::SUCCESS;

  switch (log_record.log_type()) {
  case CLogType::INSERT: {
    Record record;
    record.set_data(const_cast<char *>(data_record.data_), data_record.data_len_);
    record.set_rid(data_record.rid_);
    RC rc = table->recover_insert_record(record);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to recover insert. table=%s, log record=%s, rc=%s", table->name(),
               log_record.to_string().c_str(), strrc(rc));
      return rc;
    }
    SimpleTrxOperation oper;
    oper.type = Operation::Type::INSERT;
    oper.rid = record.rid();
    oper.table = table;
    add_oper(oper);
  } break;

  case CLogType::DELETE: {
    RC rc = Trx::delete_record(table, data_record.rid_);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to recover delete. table=%s, log record=%s, rc=%s", table->name(),
               log_record.to_string().c_str(), strrc(rc));
      return rc;
    }
  } break;

  case CLogType::MTR_COMMIT: {
    commit();
  } break;

  case CLogType::MTR_ROLLBACK: {
    rollback();
  } break;

  default: {
    ASSERT(false, "unsupported redo log. log_record=%s", log_record.to_string().c_str());
    return RC::INTERNAL;
  } break;
  }

  return RC::SUCCESS;
}

void SimpleTrx::add_oper(const SimpleTrxOperation &oper) {
  if (head_opers_.count(oper.rid) == 0)
    head_opers_[oper.rid] = oper;
  tail_opers_[oper.rid] = oper;
}
