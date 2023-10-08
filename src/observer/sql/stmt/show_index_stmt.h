#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
class ShowIndexStmt : public Stmt {
public:
  virtual StmtType type() const { return StmtType::SHOW_INDEX; }
  static RC create(Db *db, ShowIndexSqlNode &sql_node, Stmt *&stmt) {
    ShowIndexStmt *ret = new ShowIndexStmt;
    Table *table = db->find_table(sql_node.table_name.c_str());
    if (table == nullptr) {
      LOG_WARN("table %s not exitst", sql_node.table_name.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    stmt = ret;
    ret->table_ = table;
    return RC::SUCCESS;
  }

  Table *table() const { return table_; }

private:
  Table *table_;
};
