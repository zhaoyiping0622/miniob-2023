#include "sql/stmt/drop_table_stmt.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt) {
  stmt = new DropTableStmt(drop_table.relation_name);
  return RC::SUCCESS;
}
