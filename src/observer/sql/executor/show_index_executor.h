#include "common/rc.h"
#include "event/sql_event.h"
#include "sql/stmt/show_index_stmt.h"
class ShowIndexExecutor {
public:
  RC execute(SQLStageEvent *sql_event);
};
