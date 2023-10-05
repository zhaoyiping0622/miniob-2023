#include <memory>
#include <string>
#include <utility>
class SelectStmt;
class SubQueryStmt {
public:
  SubQueryStmt(std::unique_ptr<SelectStmt> stmt, std::string name) : stmt_(std::move(stmt)), name_(name) {}
  const std::string &name() const { return name_; }
  const std::unique_ptr<SelectStmt> &stmt() const { return stmt_; }

private:
  std::unique_ptr<SelectStmt> stmt_;
  std::string name_;
};
