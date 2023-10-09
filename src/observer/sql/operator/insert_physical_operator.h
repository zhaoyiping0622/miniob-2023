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
// Created by WangYunlai on 2021/6/7.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"
#include <vector>

class InsertStmt;

/**
 * @brief 插入物理算子
 * @ingroup PhysicalOperator
 */
class InsertPhysicalOperator : public PhysicalOperator {
public:
  InsertPhysicalOperator(Table *table, std::vector<std::vector<Value>> &&values);

  virtual ~InsertPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::INSERT; }

  RC open(Trx *trx) override;
  RC next(Tuple *env_tuple) override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  RC insert_all(Trx *trx, std::vector<Record> &inserted);

private:
  Table *table_ = nullptr;
  std::vector<std::vector<Value>> values_;
  size_t insert_idx = 0;
};
