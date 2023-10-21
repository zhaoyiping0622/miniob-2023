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
// Created by WangYunlai on 2022/07/05.
//

#include "sql/expr/tuple_cell.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field.h"
#include <sstream>
#include <unordered_set>

TupleCellSpec::TupleCellSpec(const char *table_name, const char *field_name, const char *alias) {
  if (table_name) {
    table_name_ = table_name;
  }
  if (field_name) {
    field_name_ = field_name;
  }
  if (alias) {
    alias_ = alias;
  } else {
    if (table_name_.empty()) {
      alias_ = field_name_;
    } else {
      alias_ = table_name_ + "." + field_name_;
    }
  }
  init_hash();
}

TupleCellSpec::TupleCellSpec(const char *alias) {
  if (alias) {
    alias_ = alias;
  }
  init_hash();
}

TupleCellSpec::TupleCellSpec(const Field &field) : TupleCellSpec(field.table_name(), field.field_name()) {
  init_hash();
}

void TupleCellSpec::init_hash() {
  static auto hasher = std::hash<std::string>{};
  table_name_hash_ = hasher(table_name_);
  field_name_hash_ = hasher(field_name_);
  alias_hash_ = hasher(alias_);
}
