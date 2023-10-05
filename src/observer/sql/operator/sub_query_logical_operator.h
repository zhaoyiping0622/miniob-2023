#pragma once

#include "sql/operator/logical_operator.h"
#include <utility>

class SubQueryLogicalOperator : public LogicalOperator {
public:
  SubQueryLogicalOperator(std::unique_ptr<LogicalOperator> &get_operator) { main_oper_ = std::move(get_operator); }
  LogicalOperatorType type() const override { return LogicalOperatorType::SUB_QUERY; }
  const std::unique_ptr<LogicalOperator> &main_oper() const { return main_oper_; }
  std::vector<std::string> &names() { return names_; }
  void add_sub_query(std::unique_ptr<LogicalOperator> oper, std::string name) {
    add_child(std::move(oper));
    names_.push_back(name);
  }

private:
  std::unique_ptr<LogicalOperator> main_oper_;
  std::vector<std::string> names_;
};
