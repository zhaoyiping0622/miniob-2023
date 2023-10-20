#include "create_view_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/tuple_cell.h"
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
  stmt = create_view_stmt;
  auto names = create_view.names;
  auto &schema = *create_view_stmt->select_stmt_->schema();
  if (names.size()) {
    if (schema.cell_num() != names.size()) {
      LOG_WARN("column num not match");
      return RC::INVALID_ARGUMENT;
    }
  } else {
    for (int i = 0; i < schema.cell_num(); i++) {
      auto &spec = schema.cell_at(i);
      std::string name;
      if (!common::is_blank(spec.field_name())) {
        name = spec.field_name();
      } else {
        // 如果是有alias，那么field_name应该是空
        name = spec.alias();
      }
      names.push_back(name);
    }
  }
  create_view_stmt->names_ = names;
  return RC::SUCCESS;
}
