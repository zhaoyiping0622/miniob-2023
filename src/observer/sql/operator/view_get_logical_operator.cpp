#include "view_get_logical_operator.h"
#include "sql/stmt/select_stmt.h"

ViewGetLogicalOperator::ViewGetLogicalOperator(View *view, std::vector<Field> fields, bool readonly, SelectStmt *select)
    : view_(view), fields_(fields), readonly_(readonly), schema_(select->schema()) {}
