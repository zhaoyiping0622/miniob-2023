#include "common/lang/serializable.h"
#include "common/rc.h"
#include "sql/parser/value.h"
#include <json/value.h>
#include <string>

class ViewFieldMeta {
public:
  std::string name() const { return name_; }
  AttrType type() const { return type_; }
  int length() const { return length_; }

  std::string table_name() const { return table_name_; }
  std::string field_name() const { return field_name_; }

  void to_json(Json::Value &json_value) const;
  static RC from_json(const Json::Value &json_value, ViewFieldMeta &field);

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
  bool updatable() const { return updatable_; }
  bool insertable() const { return insertable_; }
  bool deletable() const { return deletable_; }
  std::string name() const { return name_; }
  std::string sql() const { return sql_; }

private:
  std::string name_;
  std::string sql_;
  std::vector<ViewFieldMeta> metas_;

private:
  bool updatable_;
  bool insertable_;
  bool deletable_;
};
