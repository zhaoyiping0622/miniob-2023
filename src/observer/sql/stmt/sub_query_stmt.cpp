#include "sub_query_stmt.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"

RC SubQueryStmt::create(Db *db, const ExprSqlNode *node, const std::unordered_map<std::string, Table *> &tables,
                        Expression *&expr, SubQueryStmt *&sub_query, std::set<Field> &used_fields) {
  RC rc = RC::SUCCESS;
  if (node->type() == ExprType::LIST) {
    auto *list_node = node->get_list();
    Stmt *stmt = nullptr;
    set<Field> fields;
    rc = SelectStmt::create(db, *list_node->select, stmt, &tables, fields);
    if (rc != RC::SUCCESS)
      return rc;
    SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
    if (select_stmt->schema()->cell_num() != 1) {
      // 现在只处理子查询只有一列的情况，多列直接报错
      LOG_WARN("sub query should only have one column");
      delete stmt;
      return RC::INVALID_ARGUMENT;
    }
    sub_query = new SubQueryStmt(std::unique_ptr<SelectStmt>(select_stmt), node->name());
    expr = new ListExpr(select_stmt, node->name());
    used_fields.insert(fields.begin(), fields.end());
    return rc;
  }
  return RC::INVALID_ARGUMENT;
}
