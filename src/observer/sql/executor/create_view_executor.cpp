#include "sql/executor/create_view_executor.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "session/session.h"
#include "sql/stmt/create_view_stmt.h"
RC CreateViewExecutor::execute(SQLStageEvent *sql_event) {
  Stmt *stmt = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_VIEW, "create view executor can not run this command: %d",
         static_cast<int>(stmt->type()));
  auto *create_view_stmt = static_cast<CreateViewStmt *>(stmt);
  return session->get_current_db()->create_view(create_view_stmt->view_name().c_str(), create_view_stmt->sql().c_str(),
                                                create_view_stmt->select_stmt().release(), create_view_stmt->names());
}
