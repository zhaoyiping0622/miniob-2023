#include "view_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/value.h"
#include "sql/stmt/select_stmt.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"

static const Json::StaticString VIEW_NAME("name");
static const Json::StaticString VIEW_SQL("sql");
static const Json::StaticString VIEW_METAS("metas");
static const Json::StaticString VIEW_UPDATABLE("updatable");
static const Json::StaticString VIEW_INSERTABLE("insertable");
static const Json::StaticString VIEW_DELETABLE("deletable");

static const Json::StaticString VIEW_FIELD_NAME("name");
static const Json::StaticString VIEW_FIELD_TYPE("type");
static const Json::StaticString VIEW_FIELD_LENGTH("length");
static const Json::StaticString VIEW_FIELD_TABLE_NAME("table_name");
static const Json::StaticString VIEW_FIELD_FIELD_NAME("field_name");

void ViewFieldMeta::to_json(Json::Value &json_value) const {
  json_value[VIEW_FIELD_NAME] = name_;
  json_value[VIEW_FIELD_TYPE] = attr_type_to_string(type_);
  json_value[VIEW_FIELD_LENGTH] = length_;
  json_value[VIEW_FIELD_TABLE_NAME] = table_name_;
  json_value[VIEW_FIELD_FIELD_NAME] = field_name_;
}

RC ViewFieldMeta::from_json(const Json::Value &json_value, ViewFieldMeta &field) {
  const Json::Value view_field_name = json_value[VIEW_FIELD_NAME];
  if (!view_field_name.isString()) {
    LOG_ERROR("Invalid view field name. json value=%s", view_field_name.toStyledString().c_str());
    return RC::INTERNAL;
  }
  field.field_name_ = view_field_name.asString();

  const Json::Value view_field_type = json_value[VIEW_FIELD_TYPE];
  if (!view_field_type.isString()) {
    LOG_ERROR("Invalid view field type. json value=%s", view_field_type.toStyledString().c_str());
    return RC::INTERNAL;
  }
  field.type_ = attr_type_from_string(view_field_type.asCString());

  const Json::Value view_field_length = json_value[VIEW_FIELD_LENGTH];
  if (!view_field_length.isInt()) {
    LOG_ERROR("Invalid view field length. json value=%s", view_field_name.toStyledString().c_str());
    return RC::INTERNAL;
  }
  field.length_ = view_field_length.asInt();

  const Json::Value view_field_table_name = json_value[VIEW_FIELD_TABLE_NAME];
  if (!view_field_table_name.isInt()) {
    LOG_ERROR("Invalid view field table name. json value=%s", view_field_table_name.toStyledString().c_str());
    return RC::INTERNAL;
  }
  field.table_name_ = view_field_table_name.asString();

  const Json::Value view_field_field_name = json_value[VIEW_FIELD_FIELD_NAME];
  if (!view_field_field_name.isInt()) {
    LOG_ERROR("Invalid view field field name. json value=%s", view_field_field_name.toStyledString().c_str());
    return RC::INTERNAL;
  }
  field.field_name_ = view_field_field_name.asString();
  return RC::SUCCESS;
}

int ViewMeta::get_serial_size() const { return -1; }
void ViewMeta::to_string(std::string &output) const {}

int ViewMeta::serialize(std::ostream &os) const {
  Json::Value view_value;
  view_value[VIEW_NAME] = name_;
  view_value[VIEW_SQL] = sql_;
  view_value[VIEW_INSERTABLE] = insertable_;
  view_value[VIEW_UPDATABLE] = updatable_;
  view_value[VIEW_DELETABLE] = deletable_;
  for (int i = 0; i < metas_.size(); i++) {
    Json::Value value;
    metas_[i].to_json(value);
    view_value[VIEW_METAS].append(value);
  }

  Json::StreamWriterBuilder builder;
  Json::StreamWriter *writer = builder.newStreamWriter();

  std::streampos old_pos = os.tellp();
  writer->write(view_value, &os);
  int ret = (int)(os.tellp() - old_pos);

  delete writer;
  return ret;
}
int ViewMeta::deserialize(std::istream &is) {
  Json::Value view_value;
  Json::CharReaderBuilder builder;
  std::string errors;

  std::streampos old_pos = is.tellg();
  if (!Json::parseFromStream(builder, is, &view_value, &errors)) {
    LOG_ERROR("Failed to deserialize view meta. error=%s", errors.c_str());
    return -1;
  }
  const Json::Value &view_name_value = view_value[VIEW_NAME];
  if (!view_name_value.isString()) {
    LOG_ERROR("Invalid view name. json value=%s", view_name_value.toStyledString().c_str());
    return -1;
  }
  name_ = view_name_value.asString();

  const Json::Value &view_sql_value = view_value[VIEW_SQL];
  if (!view_name_value.isString()) {
    LOG_ERROR("Invalid view sql. json value=%s", view_sql_value.toStyledString().c_str());
    return -1;
  }
  sql_ = view_sql_value.asString();

  const Json::Value &view_metas_value = view_value[VIEW_METAS];
  if (!view_metas_value.isArray()) {
    LOG_ERROR("Invalid view metas. json value=%s", view_metas_value.toStyledString().c_str());
    return -1;
  }
  for (auto &x : view_metas_value) {
    ViewFieldMeta meta;
    RC rc = ViewFieldMeta::from_json(x, meta);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("failed to deserialize view metas");
    }
    metas_.push_back(meta);
  }

  const Json::Value &view_updatable_value = view_value[VIEW_UPDATABLE];
  if (!view_updatable_value.isBool()) {
    LOG_ERROR("Invalid view updatable. json value=%s", view_updatable_value.toStyledString().c_str());
    return -1;
  }
  updatable_ = view_updatable_value.asBool();

  const Json::Value &view_insertable_value = view_value[VIEW_INSERTABLE];
  if (!view_insertable_value.isBool()) {
    LOG_ERROR("Invalid view updatable. json value=%s", view_insertable_value.toStyledString().c_str());
    return -1;
  }
  insertable_ = view_insertable_value.asBool();

  const Json::Value &view_deletable_value = view_value[VIEW_DELETABLE];
  if (!view_deletable_value.isBool()) {
    LOG_ERROR("Invalid view deletable. json value=%s", view_deletable_value.toStyledString().c_str());
    return -1;
  }
  deletable_ = view_deletable_value.asBool();

  return (int)(is.tellg() - old_pos);
}
RC ViewMeta::create(const char *view_name, SelectStmt *select) {
  name_ = view_name;
  sql_ = select->sql();
  auto schema = select->schema();
  auto types = select->types();
  if (schema->cell_num() != types.size()) {
    LOG_ERROR("schema size not equal to types size");
    return RC::INTERNAL;
  }
  std::set<std::string> view_field_names;
  for (int i = 0; i < schema->cell_num(); i++) {
    ViewFieldMeta meta;
    auto &spec = schema->cell_at(i);
    auto &info = types[i];
    string name;
    if (!common::is_blank(spec.field_name())) {
      name = spec.field_name();
    } else {
      // 如果是有alias，那么field_name应该是空
      name = spec.alias();
    }
    if (view_field_names.count(name)) {
      return RC::INVALID_ARGUMENT;
    }
    view_field_names.insert(name);
    meta.name_ = name;
    meta.type_ = info.type;
    meta.length_ = info.length;
    if (info.raw_field.table()) {
      meta.table_name_ = info.raw_field.table_name();
      meta.field_name_ = info.raw_field.field_name();
    }
    metas_.push_back(meta);
  }
  return init(select);
}

RC ViewMeta::init(SelectStmt *select) {
  // mysql规则来判断是否可以update insert delete
  updatable_ = get_updatable(select);
  insertable_ = get_insertable(select);
  deletable_ = get_deletable(select);
  return RC::SUCCESS;
}

// TODO(zhaoyiping): 还没实现完成

// FIXME(zhaoyiping): Nondependent subqueries check
// 目前没check这个

bool ViewMeta::get_updatable(SelectStmt *select) {
  // 有聚合 直接g
  if (select->aggregation_stmt()) {
    return false;
  }
  auto schema = select->schema();
  if (select->sub_queries().size()) {
  }
}
bool ViewMeta::get_insertable(SelectStmt *select) {
  // 有聚合 直接g
  if (select->aggregation_stmt()) {
    return false;
  }
  auto schema = select->schema();
  if (select->sub_queries().size()) {
  }
}
bool ViewMeta::get_deletable(SelectStmt *select) {
  // 有聚合 直接g
  if (select->aggregation_stmt()) {
    return false;
  }
}
