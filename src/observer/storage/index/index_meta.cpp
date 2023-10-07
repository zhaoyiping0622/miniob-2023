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
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELDS("fields");
const static Json::StaticString FIELD_UNIQUE("unique");

IndexMeta::IndexMeta() {}

RC IndexMeta::init(const char *name, const std::vector<FieldMeta> &fields, bool unique) {
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_ = name;
  fields_ = fields;
  unique_ = unique;
  std::vector<std::string> all_fields;
  for (auto &field : fields) {
    all_fields.push_back(field.name());
  }
  common::merge_string(fields_name_, ", ", all_fields);
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const {
  json_value[FIELD_NAME] = name_;
  Json::Value fields;
  for (auto &field : fields_) {
    fields.append(field.name());
  }
  json_value[FIELD_FIELDS] = fields;
  json_value[FIELD_UNIQUE] = unique_;
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index) {
  const Json::Value &name_value = json_value[FIELD_NAME];
  const Json::Value &fields_value = json_value[FIELD_FIELDS];
  const Json::Value &unique_value = json_value[FIELD_UNIQUE];

  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!fields_value.isArray()) {
    LOG_ERROR("Field name of index [%s] is not a array. json value=%s", name_value.asCString(),
              fields_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  std::vector<FieldMeta> fields_meta;

  for (auto field_value : fields_value) {
    if (!field_value.isString()) {
      LOG_ERROR("Field name of index [%s] is not a string. json value=%s", name_value.asCString(),
                field_value.toStyledString().c_str());
    }
    const FieldMeta *field = table.field(field_value.asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields_meta.push_back(*field);
  }

  if (!unique_value.isBool()) {
    LOG_ERROR("unique is not a boolean. json value=%s", unique_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  bool unique = unique_value.asBool();

  return index.init(name_value.asCString(), fields_meta, unique);
}

const char *IndexMeta::name() const { return name_.c_str(); }

void IndexMeta::desc(std::ostream &os) const { os << "index name=" << name_ << ", field={" << fields_name_ << "}"; }
