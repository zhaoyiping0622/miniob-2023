#pragma once
#include "common/rc.h"
#include "view_meta.h"

class SelectStmt;
class Db;
class TableMeta;

class View {
public:
  View();
  ~View();
  RC create_view(Db *db, const char *view_file_name, const char *view_name, SelectStmt *select, std::vector<std::string> &names);
  RC open(Db *db, const char *meta_file);
  RC open_without_parse(const char *meta_file);
  RC parse_sql(Db *db);

public:
  std::string name() { return view_meta_.name(); }
  RC select(std::unique_ptr<SelectStmt> &select);
  ViewMeta &view_meta() { return view_meta_; }

public:
  void init_table_meta();
  TableMeta &table_meta();

private:
  // 模拟table
  std::unique_ptr<TableMeta> table_meta_ = nullptr;

private:
  ViewMeta view_meta_;
  std::unique_ptr<SelectStmt> select_;
  RC parse_sql(Db *db, const char *sql);
  Db *db_;
};
