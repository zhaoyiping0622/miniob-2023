#pragma once
#include "common/rc.h"
#include "view_meta.h"

class SelectStmt;
class Db;
class TableMeta;

class View {
public:
  RC create_view(Db *db, const char *view_file_name, const char *view_name, SelectStmt *select);
  RC open(Db *db, const char *meta_file);
  RC open_without_parse(const char *meta_file);
  RC parse_sql(Db *db);
  std::string name() { return view_meta_.name(); }
  void init_table_meta();
  TableMeta &table_meta();
  SelectStmt *select() { return select_.get(); }

private:
  // 模拟table
  std::unique_ptr<TableMeta> table_meta_ = nullptr;

private:
  ViewMeta view_meta_;
  std::shared_ptr<SelectStmt> select_;
  RC parse_sql(Db *db, const char *sql);
  Db *db_;
};
