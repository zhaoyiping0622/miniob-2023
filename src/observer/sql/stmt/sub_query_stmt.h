#pragma once

#include "sql/parser/parse_defs.h"
#include "storage/db/db.h"
#include <memory>
#include <string>
#include <utility>

class SelectStmt;
class SubQueryStmt {
public:
  SubQueryStmt(std::unique_ptr<SelectStmt> stmt, std::string name) : stmt_(std::move(stmt)), name_(name) {}
  const std::string &name() const { return name_; }
  const std::unique_ptr<SelectStmt> &stmt() const { return stmt_; }

  static RC create(Db *db, const ExprSqlNode *node, const std::unordered_map<std::string, Table *> &tables,
                   Expression *&expr, SubQueryStmt *&sub_query, std::set<Field> &used_fields);

private:
  std::unique_ptr<SelectStmt> stmt_;
  std::string name_;
};
