#include "create_view_stmt.h"
#include "common/rc.h"
#include "sql/stmt/select_stmt.h"
RC CreateViewStmt::create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt) {
  Stmt *select_stmt;
  RC rc = Stmt::create_stmt(db, *create_view.select, select_stmt);
  if (rc != RC::SUCCESS)
    return rc;
  auto *create_view_stmt = new CreateViewStmt;
  create_view_stmt->view_name_ = create_view.view_name;
  create_view_stmt->sql_ = create_view.select_sql;
  create_view_stmt->select_stmt_.reset(static_cast<SelectStmt *>(select_stmt));
  return RC::SUCCESS;
}
