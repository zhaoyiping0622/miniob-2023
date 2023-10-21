#include "sql/stmt/join_stmt.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "storage/table/table.h"
#include <cassert>

RC get_table(Db *db, const char *table_name, Table *&table) {
  if (nullptr == table_name) {
    LOG_ERROR("invalid argument. relation name is null.");
    return RC::INVALID_ARGUMENT;
  }

  Table *tmp = db->find_table(table_name);
  if (nullptr == tmp) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  table = tmp;
  return RC::SUCCESS;
}

RC JoinStmt::create(Db *db, JoinSqlNode *sql_node, JoinStmt *&stmt, vector<std::string> &tables,
                    unordered_map<string, Table *> &table_map) {
  Table *table;
  RC rc = get_table(db, sql_node->relation.c_str(), table);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  string alias_name = table->name();
  if (sql_node->alias.size())
    alias_name = sql_node->alias;
  if (table_map.count(alias_name) == 0) {
    tables.push_back(alias_name);
    table_map[alias_name] = table;
  } else {
    LOG_WARN("table name duplicated");
    return RC::INVALID_ARGUMENT;
  }

  stmt = new JoinStmt;
  stmt->alias_ = alias_name;
  stmt->table_ = table;
  if (sql_node->sub_join == nullptr) {
    return RC::SUCCESS;
  }
  JoinStmt *sub_join = nullptr;
  rc = create(db, sql_node->sub_join, sub_join, tables, table_map);
  if (rc != RC::SUCCESS) {
    delete stmt;
    return rc;
  }
  stmt->sub_join_.reset(sub_join);
  if (sql_node->join_conditions) {
    Expression *expr;
    rc = ConjunctionExpr::create(db, nullptr, &table_map, sql_node->join_conditions, expr, nullptr);
    if (rc != RC::SUCCESS || expr->value_type() != BOOLEANS) {
      delete stmt;
      return rc;
    }
    stmt->condition_.reset(static_cast<ConjunctionExpr *>(expr));
  }
  return rc;
}
