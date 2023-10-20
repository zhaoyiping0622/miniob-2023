#pragma once

#include "common/lang/serializable.h"
#include "common/rc.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/value.h"
#include "storage/field/field.h"
#include <json/value.h>
#include <string>

class SelectStmt;
class Db;

class ViewFieldMeta {
public:
  std::string name() const { return name_; }
  AttrType type() const { return type_; }
  int length() const { return length_; }

public:
  std::string table_name() const { return table_name_; }
  std::string field_name() const { return field_name_; }
  Field raw_field();
  bool is_reference() const { return table_name_.size() != 0; }

public:
  void to_json(Json::Value &json_value) const;
  static RC from_json(const Json::Value &json_value, ViewFieldMeta &field);

  friend class ViewMeta;

  TupleCellSpec get_tuple_spec() const;

private:
  std::string name_;
  AttrType type_;
  int length_;

  std::string table_name_;
  std::string field_name_;
};

class ViewMeta : public common::Serializable {
public:
  virtual int serialize(std::ostream &os) const override;
  virtual int deserialize(std::istream &is) override;
  virtual int get_serial_size() const override;
  virtual void to_string(std::string &output) const override;

public:
  RC create(const char *view_name, SelectStmt *select, std::vector<std::string> &names);
  RC init(SelectStmt *select);

public:
  bool updatable() const { return updatable_; }
  bool insertable() const { return insertable_; }
  bool deletable() const { return deletable_; }
  std::string &name() { return name_; }
  std::string &sql() { return sql_; }
  std::vector<ViewFieldMeta> &metas() { return metas_; };

  Table *get_insert_table(Db *db, std::vector<int> &order);
  Table *get_delete_table(Db *db);
  Table *get_update_table(Db *db, std::vector<std::string> fields, std::vector<Field> &view_field_metas);

public:
  ViewFieldMeta *field(const char *name);

private:
  std::string name_;
  std::string sql_;
  std::vector<ViewFieldMeta> metas_;

private:
  bool updatable_;
  bool insertable_;
  bool deletable_;
  bool has_aggregation_;

private:
  bool get_updatable(SelectStmt *select);
  bool get_insertable(SelectStmt *select);
  bool get_deletable(SelectStmt *select);
};
