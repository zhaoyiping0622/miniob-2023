#include "rename_logical_operator.h"
#include "sql/operator/logical_operator.h"
void RenameLogicalOperator::add_rename(TupleCellSpec from, TupleCellSpec to) { spec_map_.emplace_back(from, to); }
