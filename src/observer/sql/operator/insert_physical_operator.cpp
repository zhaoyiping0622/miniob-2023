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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/insert_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(Table *table, vector<vector<Value>> &&values)
    : table_(table), values_(std::move(values)) {}

RC InsertPhysicalOperator::open(Trx *trx) {
  vector<Record> inserted;
  RC rc;
  rc = insert_all(trx, inserted);
  return rc;
}

RC InsertPhysicalOperator::insert_all(Trx *trx, vector<Record> &inserted) {
  inserted.reserve(values_.size());
  for (auto &value : values_) {
    Record record;
    RC rc = table_->make_record(static_cast<int>(value.size()), value.data(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }

    rc = trx->insert_record(table_, record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      return rc;
    }

    inserted.push_back(record);
  }
  return RC::SUCCESS;
}

RC InsertPhysicalOperator::next(Tuple *env_tuple) { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
