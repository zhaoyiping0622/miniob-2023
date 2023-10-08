#include "sql/operator/update_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/tuple.h"
#include "sql/parser/value.h"
#include <cstring>

RC UpdatePhysicalOperator::open(Trx *trx) {
  RC rc = children_[0]->open(trx);
  if (rc != RC::SUCCESS)
    return rc;
  vector<vector<char>> records;
  while ((rc = children_[0]->next(nullptr)) == RC::SUCCESS) {
    // FIXME(zhaoyiping): 这里之后要统一改成接口
    auto *tuple = static_cast<RowTuple *>(children_[0]->current_tuple());
    auto &record = tuple->record();
    vector<char> r(table_->table_meta().record_size());
    memcpy(r.data(), record.data(), r.size());
    rc = table_->delete_record(record);
    if (rc != RC::SUCCESS) {
      auto rc1 = insert_all(records);
      if (rc1 != RC::SUCCESS) {
        LOG_ERROR("failed to rollback update");
      }
      return rc;
    }
    records.push_back(r);
  }
  children_[0]->close();
  if (rc != RC::RECORD_EOF) {
    return rc;
  }
  vector<RID> inserted;
  for (auto it = records.rbegin(); it != records.rend(); it++) {
    RID rid;
    rc = update(*it, rid);
    if (rc != RC::SUCCESS) {
      RC rc1 = RC::SUCCESS;
      rc1 = remove_all(inserted);
      if (rc1 != RC::SUCCESS) {
        LOG_ERROR("failed to rollback update, error in remove inserted");
      }
      rc1 = insert_all(records);
      if (rc1 != RC::SUCCESS) {
        LOG_ERROR("failed to rollback update, error in insert deleted");
      }
      return rc;
    }
    inserted.push_back(rid);
  }
  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::insert_all(vector<vector<char>> &v) {
  RC rc_ret = RC::SUCCESS;
  for (auto &x : v) {
    RID rid;
    RC rc = insert(x, rid);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("fail to insert all");
      if (rc_ret == RC::SUCCESS) {
        rc_ret = rc;
      }
    }
  }
  return rc_ret;
}

RC UpdatePhysicalOperator::insert(vector<char> &v, RID &rid) {
  Record record;
  RC rc = table_->make_record(v.data(), v.size(), record);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("fail to make record");
    return rc;
  }
  rc = table_->insert_record(record);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("fail to insert record");
    return rc;
  }
  rid = record.rid();
  return rc;
}

RC UpdatePhysicalOperator::remove_all(const vector<RID> &rids) {
  RC rc_ret = RC::SUCCESS;
  for (auto &rid : rids) {
    RC rc = table_->delete_record(rid);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("fail to delete record");
      if (rc_ret == RC::SUCCESS) {
        rc_ret = rc;
      }
    }
  }
  return rc_ret;
}

RC UpdatePhysicalOperator::update(vector<char> v, RID &rid) {
  for (auto &unit : units_) {
    const auto *meta = unit.field.meta();
    int offset = meta->offset();
    memcpy(v.data() + offset, unit.value.data(), attr_type_to_size(meta->type()));
  }
  return insert(v, rid);
}
