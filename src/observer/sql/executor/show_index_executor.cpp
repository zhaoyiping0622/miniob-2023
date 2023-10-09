#include "show_index_executor.h"
#include "common/rc.h"
#include "event/session_event.h"
#include "session/session.h"
#include "sql/expr/tuple.h"
#include "sql/operator/string_list_physical_operator.h"
#include <string>

RC ShowIndexExecutor::execute(SQLStageEvent *sql_event) {
  auto *stmt = sql_event->stmt();
  SessionEvent *session_event = sql_event->session_event();
  Session *session = session_event->session();
  ASSERT(stmt->type() == StmtType::SHOW_INDEX, "show index executor can not run this command: %d",
         static_cast<int>(stmt->type()));
  auto *show_index_stmt = static_cast<ShowIndexStmt *>(stmt);
  SqlResult *sql_result = session_event->sql_result();
  TupleSchema tuple_schema;
  auto *table = show_index_stmt->table();

  tuple_schema.append_cell(TupleCellSpec("Table"));
  tuple_schema.append_cell(TupleCellSpec("Non_unique"));
  tuple_schema.append_cell(TupleCellSpec("Key_name"));
  tuple_schema.append_cell(TupleCellSpec("Seq_in_index"));
  tuple_schema.append_cell(TupleCellSpec("Column_name"));

  sql_result->set_tuple_schema(tuple_schema);

  auto oper = new StringListPhysicalOperator;
  const TableMeta &table_meta = table->table_meta();
  for (int i = 0; i < table_meta.index_num(); i++) {
    const auto &index_meta = table_meta.index(i);
    auto &fields = index_meta->fields();
    int index = 1;
    for (int i = 0; i < fields.size(); i++) {
      if (!fields[i].visible())
        continue;
      oper->append({
          table->name(),
          index_meta->unique() ? "0" : "1",
          index_meta->name(),
          to_string(index++),
          fields[i].name(),
      });
    }
  }

  sql_result->set_operator(unique_ptr<PhysicalOperator>(oper));
  return RC::SUCCESS;
}
