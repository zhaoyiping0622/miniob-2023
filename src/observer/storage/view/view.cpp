#include "storage/view/view.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/parse.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "storage/db/db.h"
#include <fcntl.h>

// 搞出来select各项，然后初始化ViewMeta，然后把它持久化
RC View::create_view(const char *view_file_name, const char *view_name, SelectStmt *select) {
  select_.reset(select);
  int fd = ::open(view_file_name, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
  if (fd < 0) {
    if (EEXIST == errno) {
      LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s", view_file_name, strerror(errno));
      return RC::SCHEMA_VIEW_EXIST;
    }
    LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s", view_file_name, errno, strerror(errno));
    return RC::IOERR_OPEN;
  }

  close(fd);
  RC rc = view_meta_.create(view_name, select);
  if (rc != RC::SUCCESS)
    return rc;
  std::fstream fs;
  fs.open(view_file_name, std::ios_base::out | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", view_file_name, strerror(errno));
    return RC::IOERR_OPEN;
  }

  // 记录元数据到文件中
  view_meta_.serialize(fs);
  fs.close();
  return rc;
}

// 直接打开文件，初始化view_meta，使用sql生成stmt
RC View::open(Db *db, const char *meta_file) {
  RC rc = RC::SUCCESS;
  rc = open_without_parse(meta_file);
  if (rc != RC::SUCCESS)
    return rc;
  return parse_sql(db);
}

RC View::open_without_parse(const char *meta_file) {
  std::fstream fs;
  fs.open(meta_file, std::ios_base::in | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file, strerror(errno));
    return RC::IOERR_OPEN;
  }
  if (view_meta_.deserialize(fs) < 0) {
    LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file);
    fs.close();
    return RC::INTERNAL;
  }
  fs.close();
  return RC::SUCCESS;
}
RC View::parse_sql(Db *db) {
  RC rc = RC::SUCCESS;
  rc = parse_sql(db, view_meta_.sql().c_str());
  if (rc != RC::SUCCESS)
    return rc;
  return view_meta_.init(select_.get());
}

RC View::parse_sql(Db *db, const char *sql) {
  ParsedSqlResult sql_result;
  RC rc = RC::SUCCESS;
  rc = parse(sql, &sql_result);
  if (rc != RC::SUCCESS)
    return rc;
  if (sql_result.sql_nodes().size() != 1)
    return RC::INTERNAL;
  Stmt *stmt;
  rc = Stmt::create_stmt(db, *sql_result.sql_nodes()[0], stmt);
  if (rc != RC::SUCCESS)
    return rc;
  if (stmt->type() != StmtType::SELECT) {
    LOG_ERROR("view sql is not SELECT, type is %s", stmt_type_name(stmt->type()));
    return RC::INTERNAL;
  }
  select_.reset(static_cast<SelectStmt *>(stmt));
  return RC::SUCCESS;
}
