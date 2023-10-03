/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/aggregation_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/join_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include <functional>
#include <memory>

SelectStmt::~SelectStmt() {}

static void wildcard_fields(Table *table, std::vector<std::unique_ptr<Expression>> &field_metas) {
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.emplace_back(new FieldExpr(table, table_meta.field(i)));
  }
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt) {
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;
  ConjunctionExprSqlNode *conditions = nullptr;
  auto add_conjunction = [&](ConjunctionExprSqlNode *node) {
    if (conditions == nullptr)
      conditions = node;
    else {
      conditions = new ConjunctionExprSqlNode(ConjunctionType::AND, conditions, node);
    }
  };
  RC rc = RC::SUCCESS;
  unique_ptr<JoinStmt> join_stmt;
  if (select_sql.tables) {
    JoinStmt *join = nullptr;
    rc = JoinStmt::create(db, select_sql.tables, join, tables, table_map);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to create join stmt");
    }
    join_stmt.reset(join);
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // 处理groupby
  set<Field> groupbys;
  for (auto *x : select_sql.groupbys) {
    Expression *field;
    RC rc = FieldExpr::create(db, default_table, &table_map, x, field);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (field->type() != ExprType::FIELD) {
      LOG_ERROR("FieldExpr::create not get field expr, get %s", field->type());
      return RC::INTERNAL;
    }
    groupbys.insert(static_cast<FieldExpr *>(field)->field());
    delete field;
  }

  shared_ptr<TupleSchema> schema = make_shared<TupleSchema>();

  unique_ptr<AggregationStmt> aggregation_stmt = make_unique<AggregationStmt>();

  aggregation_stmt->group_fields() = groupbys;

  ExprGenerator subexpr_generator = [&](const ExprSqlNode *node, Expression *&expr) -> RC {
    // 处理named的情况，提取aggregation子句
    if (node->type() != ExprType::NAMED) {
      LOG_ERROR("subexpr_generator get node type %d", node->type());
      return RC::INTERNAL;
    }
    auto named_node = node->get_named();
    string name = named_node->name;
    AggregationExprSqlNode *aggr_sql_node = named_node->child;
    AggregationType type = aggr_sql_node->type;
    auto &children = aggr_sql_node->children;
    if (children.size() != 1) {
      LOG_WARN("aggregation operator not support multiple arguments");
      return RC::INVALID_ARGUMENT;
    }
    ExprSqlNode *sub_expr_sql_node = children[0];
    Expression *sub_expr = nullptr;
    if (sub_expr_sql_node->type() != ExprType::STAR) {
      RC rc = Expression::create(db, default_table, &table_map, sub_expr_sql_node, sub_expr, nullptr);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to parse sub_expr");
        return rc;
      }
    } else {
      if (aggr_sql_node->type != AggregationType::AGGR_COUNT) {
        LOG_WARN("aggregation on '*', type is not count");
        return RC::INTERNAL;
      }
      sub_expr = new ValueExpr(Value(0));
    }
    AggregationUnit *aggr_unit = new AggregationUnit(name, type, sub_expr);
    auto value_type = aggr_unit->value_type();
    if (value_type == UNDEFINED) {
      delete aggr_unit;
      LOG_WARN("aggregate on unsupport type");
      return RC::INVALID_ARGUMENT;
    }
    expr = new NamedExpr(value_type, TupleCellSpec(name.c_str()));
    aggregation_stmt->add_aggregation_unit(aggr_unit);
    return RC::SUCCESS;
  };

  // collect query fields in `select` statement
  std::vector<std::unique_ptr<Expression>> expressions;
  for (int i = 0; i < select_sql.attributes.size(); i++) {
    const ExprSqlNode *expression = select_sql.attributes[i];

    if (expression->type() == ExprType::STAR) {
      for (Table *table : tables) {
        wildcard_fields(table, expressions);
      }
    } else {
      Expression *expr;
      RC rc = Expression::create(db, default_table, &table_map, expression, expr, &subexpr_generator);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to parse expression, rc=%s", strrc(rc));
        return rc;
      }
      expressions.emplace_back(expr);
    }
  }

  set<Field> used_fields = groupbys;

  vector<set<Field>> reference_fields(expressions.size());
  set<Field> attr_used_fields = groupbys;

  auto append_cell = [&](Expression *expression) {
    if (expression->type() == ExprType::FIELD) {
      Field &field = static_cast<FieldExpr *>(expression)->field();
      schema->append_cell(field.table_name(), field.field_name());
    } else {
      schema->append_cell(expression->name().c_str());
    }
  };

  for (int i = 0; i < expressions.size(); i++) {
    auto fields = expressions[i]->reference_fields();
    attr_used_fields.insert(fields.begin(), fields.end());
    append_cell(expressions[i].get());
    reference_fields[i].swap(fields);
  }

  set<Field> filter_used_fields;

  // create filter statement in `where` statement
  unique_ptr<FilterStmt> filter_stmt;
  if (select_sql.conditions != nullptr) {
    add_conjunction(select_sql.conditions);
  }
  if (conditions != nullptr) {
    FilterStmt *stmt = nullptr;
    RC rc = FilterStmt::create(db, default_table, &table_map, conditions, stmt, nullptr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
    filter_stmt.reset(stmt);
    auto &expr = filter_stmt->filter_expr();
    filter_used_fields = expr->reference_fields();
  }

  used_fields.insert(filter_used_fields.begin(), filter_used_fields.end());

  auto check_fields = [&](const set<Field> &fields) -> RC {
    if (!aggregation_stmt->has_aggregate())
      return RC::SUCCESS;
    for (auto x : fields) {
      if (groupbys.count(x) == 0) {
        LOG_WARN("expressions use field not in groupby expression, use=%s.%s", x.table_name(), x.field_name());
        return RC::FIELD_NOT_IN_GROUP_BY;
      }
    }
    return RC::SUCCESS;
  };

  unique_ptr<FilterStmt> having_stmt;
  if (select_sql.having_conditions != nullptr) {
    FilterStmt *stmt = nullptr;
    RC rc = FilterStmt::create(db, default_table, &table_map, select_sql.having_conditions, stmt, &subexpr_generator);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
    auto fields = stmt->filter_expr()->reference_fields();
    having_stmt.reset(stmt);
    if ((rc = check_fields(fields)) != RC::SUCCESS) {
      return rc;
    }
  }

  unique_ptr<OrderByStmt> orderby(new OrderByStmt);
  for (auto &node : select_sql.orderbys) {
    OrderByUnit unit;
    unit.order = node->order;
    Expression *expr;
    RC rc = FieldExpr::create(db, default_table, &table_map, node->field, expr);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to parse field expr");
      return rc;
    }
    auto fields = expr->reference_fields();
    if ((rc = check_fields(fields)) != RC::SUCCESS) {
      LOG_WARN("orderby use fields not in groupby");
      return rc;
    }
    used_fields.insert(fields.begin(), fields.end());
    unit.expr.reset(expr);
    orderby->add_order_by_unit(unit);
  }

  if ((rc = check_fields(attr_used_fields)) != RC::SUCCESS) {
    return rc;
  }

  LOG_INFO("got %d tables in from stmt and %d fields in query stmt", tables.size(), expressions.size());

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();
  // TODO add expression copy
  select_stmt->tables_.swap(tables);
  select_stmt->used_fields_ = used_fields;
  select_stmt->reference_fields_.swap(reference_fields);
  select_stmt->filter_stmt_ = std::move(filter_stmt);
  select_stmt->having_stmt_ = std::move(having_stmt);
  select_stmt->expressions_.swap(expressions);
  select_stmt->schema_.swap(schema);
  select_stmt->aggregation_stmt_.swap(aggregation_stmt);
  select_stmt->orderby_stmt_.swap(orderby);
  select_stmt->join_stmt_.swap(join_stmt);

  stmt = select_stmt;
  return RC::SUCCESS;
}
