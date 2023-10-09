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
  vector<vector<Value>> value_list;
  trx_ = trx;
  while ((rc = children_[0]->next(nullptr)) == RC::SUCCESS) {
    // FIXME(zhaoyiping): 这里之后要统一改成接口
    Record *record;
    Tuple *tuple = children_[0]->current_tuple();
    rc = tuple->get_record(table_, record);
    if (rc != RC::SUCCESS)
      return rc;
    vector<char> r(table_->table_meta().record_size());
    memcpy(r.data(), record->data(), r.size());
    rc = trx->delete_record(table_, *record);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    deleted_records_.push_back(r);
    vector<Value> values;
    for (auto &unit : units_) {
      Value value;
      rc = unit.value->get_value(*tuple, value);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      values.push_back(value);
    }
    value_list.push_back(values);
  }
  children_[0]->close();
  if (rc != RC::RECORD_EOF) {
    return rc;
  }
  for (int i = 0; i < deleted_records_.size(); i++) {
    RID rid;
    rc = update(deleted_records_[i], value_list[i], rid);
    if (rc != RC::SUCCESS) {
      return rc;
    }
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
  rc = trx_->insert_record(table_, record);
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
    RC rc = trx_->delete_record(table_, rid);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("fail to delete record");
      if (rc_ret == RC::SUCCESS) {
        rc_ret = rc;
      }
    }
  }
  return rc_ret;
}

RC UpdatePhysicalOperator::update(vector<char> v, vector<Value> &values, RID &rid) {
  int &null_value = *(int *)(v.data() + table_->table_meta().null_field_meta()->offset());
  RC rc = RC::SUCCESS;
  for (int i = 0; i < values.size(); i++) {
    auto &unit = units_[i];
    Value value;
    if (!values[i].get_only(value)) {
      LOG_WARN("list not has one value");
      return RC::INVALID_ARGUMENT;
    }
    const auto *meta = unit.field.meta();
    if (value.attr_type() != meta->type()) {
      if (value.attr_type() == NULLS) {
        if (!meta->nullable()) {
          LOG_WARN("field %s should not be null", meta->name());
          return RC::INVALID_ARGUMENT;
        }
      } else if (meta->type() == TEXTS) {
        int page_of;
        rc = table_->add_text(value.get_string().c_str(), page_of);
        if (rc != RC::SUCCESS)
          return rc;
        value.set_int(page_of);
      } else if (!Value::convert(value.attr_type(), meta->type(), value)) {
        LOG_WARN("failed to convert update value");
        return RC::INVALID_ARGUMENT;
      }
    }
    int offset = meta->offset();
    memcpy(v.data() + offset, value.data(), attr_type_to_size(meta->type()));
    if (value.is_null()) {
      null_value |= 1 << meta->index();
    } else {
      null_value &= (-1) ^ (1 << meta->index());
    }
  }
  return insert(v, rid);
}
