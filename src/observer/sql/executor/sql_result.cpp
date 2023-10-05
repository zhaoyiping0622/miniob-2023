/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/11/18.
//

#include "sql/executor/sql_result.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "session/session.h"
#include "storage/trx/trx.h"

SqlResult::SqlResult(Session *session) : session_(session) {}

void SqlResult::set_tuple_schema(const TupleSchema &schema) { tuple_schema_ = schema; }

RC SqlResult::open() {
  if (nullptr == operator_) {
    return RC::INVALID_ARGUMENT;
  }

  Trx *trx = session_->current_trx();
  trx->start_if_need();
  RC rc = operator_->open(trx);
  if (rc != RC::SUCCESS)
    return rc;
  while ((rc = operator_->next(nullptr)) == RC::SUCCESS) {
    std::vector<Value> values(tuple_schema_.cell_num());
    Tuple *sub_tuple = operator_->current_tuple();
    for (int i = 0; i < values.size(); i++) {
      rc = sub_tuple->cell_at(i, values[i]);
      if (rc != RC::SUCCESS)
        return rc;
    }
    records_.push_back(values);
  }
  if (rc != RC::RECORD_EOF) {
    return rc;
  }
  return RC::SUCCESS;
}

RC SqlResult::close() {
  if (nullptr == operator_) {
    return RC::INVALID_ARGUMENT;
  }
  RC rc = operator_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to close operator. rc=%s", strrc(rc));
  }

  operator_.reset();

  if (session_ && !session_->is_trx_multi_operation_mode()) {
    if (rc == RC::SUCCESS) {
      rc = session_->current_trx()->commit();
    } else {
      RC rc2 = session_->current_trx()->rollback();
      if (rc2 != RC::SUCCESS) {
        LOG_PANIC("rollback failed. rc=%s", strrc(rc2));
      }
    }
  }
  return rc;
}

RC SqlResult::next_tuple(Tuple *&tuple) {
  RC rc = RC::SUCCESS;
  idx_++;
  if (idx_ == records_.size())
    return RC::RECORD_EOF;
  if (idx_ == 0) {
    vector<TupleCellSpec> speces(tuple_schema_.cell_num());
    for (int i = 0; i < speces.size(); i++)
      speces[i] = tuple_schema_.cell_at(i);
    tuple_.set_speces(speces);
  }
  tuple_.set_cells(records_[idx_]);
  tuple = &tuple_;
  return rc;
}

void SqlResult::set_operator(std::unique_ptr<PhysicalOperator> oper) {
  ASSERT(operator_ == nullptr, "current operator is not null. Result is not closed?");
  operator_ = std::move(oper);
}
