#pragma once

#include "event/sql_event.h"
class SQLStageEvent;

class DropTableExecutor {
public:
  DropTableExecutor() = default;
  virtual ~DropTableExecutor() = default;

  RC execute(SQLStageEvent *sql_event);
};
