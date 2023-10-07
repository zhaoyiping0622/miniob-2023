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
// Created by Wangyunlai on 2021/5/12.
//

#pragma once

#include "common/rc.h"
#include <string>
#include <vector>

class TableMeta;
class FieldMeta;
class Field;

namespace Json {
class Value;
} // namespace Json

/**
 * @brief 描述一个索引
 * @ingroup Index
 * @details 一个索引包含了表的哪些字段，索引的名称等。
 * 如果以后实现了多种类型的索引，还需要记录索引的类型，对应类型的一些元数据等
 */
class IndexMeta {
public:
  IndexMeta();

  RC init(const char *name, const std::vector<FieldMeta> &fields, bool unique);

public:
  const char *name() const;
  const std::vector<FieldMeta> &fields() const { return fields_; }
  const std::string &fields_name() const { return fields_name_; }
  bool unique() const { return unique_; }

  void desc(std::ostream &os) const;

public:
  void to_json(Json::Value &json_value) const;
  static RC from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index);

protected:
  std::string name_; // index's name
  std::vector<FieldMeta> fields_;
  std::string fields_name_;
  bool unique_ = false;
};
