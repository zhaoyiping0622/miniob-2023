#pragma once

#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include <memory>

class CreateViewStmt : public Stmt {
public:
  virtual ~CreateViewStmt() = default;
  StmtType type() const override { return StmtType::CREATE_VIEW; }

  static RC create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt);

private:
  std::string view_name_;
  std::unique_ptr<SelectStmt> select_stmt_;
  std::string sql_;
};
