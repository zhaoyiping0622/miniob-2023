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
// Created by Meiyi
//

#pragma once

#include <memory>
#include <stddef.h>
#include <string>
#include <utility>
#include <vector>

#include "sql/parser/value.h"

class Expression;
struct ParsedSqlNode;

/**
 * @brief 表达式类型
 * @ingroup Expression
 */
enum class ExprType {
  NONE,
  STAR,        ///< 星号，表示所有字段
  FIELD,       ///< 字段。在实际执行时，根据行数据内容提取对应字段的值
  VALUE,       ///< 常量值
  CAST,        ///< 需要做类型转换的表达式
  COMPARISON,  ///< 需要做比较的表达式
  CONJUNCTION, ///< 多个表达式使用同一种关系(AND或OR)来联结
  ARITHMETIC,  ///< 算术运算
  NAMED,       ///< 聚合操作
  FUNCTION,    ///< 函数操作
  CONTAIN,     ///< in和not in操作
  EXISTS,      ///< exist and not exist
  LIKE,        ///< like
  NULL_CHECK,  ///< is null 和 is not null
  LIST,        ///<
  SET,         ///<
};

enum class ConjunctionType {
  AND,
  OR,
  SINGLE,
};

enum class ArithmeticType {
  ADD,
  SUB,
  MUL,
  DIV,
  NEGATIVE,
};

enum class FunctionType {
  LENGTH,
  ROUND,
  DATE_FORMAT,
};

enum class ContainType {
  IN,
  NOT_IN,
};

enum class Order {
  ASC,
  DESC,
};

/**
 * @defgroup SQLParser SQL Parser 
 */

/**
 * @brief 描述比较运算符
 * @ingroup SQLParser
 */
enum CompOp {
  EQUAL_TO,    ///< "="
  LESS_EQUAL,  ///< "<="
  NOT_EQUAL,   ///< "<>"
  LESS_THAN,   ///< "<"
  GREAT_EQUAL, ///< ">="
  GREAT_THAN,  ///< ">"
  NO_OP
};

struct ExprSqlNode;
struct StarExprSqlNode;
struct FieldExprSqlNode;
struct ValueExprSqlNode;
struct ComparisonExprSqlNode;
struct ConjunctionExprSqlNode;
struct ArithmeticExprSqlNode;
struct AggregationExprSqlNode;
struct NamedExprSqlNode;
struct FunctionExprSqlNode;
struct SelectSqlNode;
struct ContainExprSqlNode;
struct ExistsExprSqlNode;
struct LikeExprSqlNode;
struct ListExprSqlNode;
struct SetExprSqlNode;
struct NullCheckExprSqlNode;

class ExprSqlNode {
private:
  ExprType type_;
  union {
    StarExprSqlNode *star;
    FieldExprSqlNode *field;
    ValueExprSqlNode *value;
    ComparisonExprSqlNode *comparison;
    ConjunctionExprSqlNode *conjunction;
    ArithmeticExprSqlNode *arithmetic;
    NamedExprSqlNode *named;
    FunctionExprSqlNode *function;
    ContainExprSqlNode *contain;
    ExistsExprSqlNode *exists;
    LikeExprSqlNode *like;
    ListExprSqlNode *list;
    SetExprSqlNode *set;
    NullCheckExprSqlNode *null;
  } expr_;
  std::string name_;

public:
  ExprSqlNode(StarExprSqlNode *star) : type_(ExprType::STAR) { expr_.star = star; }
  ExprSqlNode(FieldExprSqlNode *field) : type_(ExprType::FIELD) { expr_.field = field; }
  ExprSqlNode(ValueExprSqlNode *value) : type_(ExprType::VALUE) { expr_.value = value; }
  ExprSqlNode(ComparisonExprSqlNode *comparison) : type_(ExprType::COMPARISON) { expr_.comparison = comparison; }
  ExprSqlNode(ConjunctionExprSqlNode *conjunction) : type_(ExprType::CONJUNCTION) { expr_.conjunction = conjunction; }
  ExprSqlNode(ArithmeticExprSqlNode *arithmetic) : type_(ExprType::ARITHMETIC) { expr_.arithmetic = arithmetic; }
  ExprSqlNode(NamedExprSqlNode *named) : type_(ExprType::NAMED) { expr_.named = named; }
  ExprSqlNode(FunctionExprSqlNode *function) : type_(ExprType::FUNCTION) { expr_.function = function; }
  ExprSqlNode(ContainExprSqlNode *contain) : type_(ExprType::CONTAIN) { expr_.contain = contain; }
  ExprSqlNode(ExistsExprSqlNode *exists) : type_(ExprType::EXISTS) { expr_.exists = exists; }
  ExprSqlNode(LikeExprSqlNode *like) : type_(ExprType::LIKE) { expr_.like = like; }
  ExprSqlNode(ListExprSqlNode *list) : type_(ExprType::LIST) { expr_.list = list; }
  ExprSqlNode(SetExprSqlNode *set) : type_(ExprType::SET) { expr_.set = set; }
  ExprSqlNode(NullCheckExprSqlNode *null) : type_(ExprType::NULL_CHECK) { expr_.null = null; }
  ~ExprSqlNode();
  ExprType type() const { return type_; }
  const std::string &name() const { return name_; }
  void set_name(std::string name) { name_ = name; }
  FieldExprSqlNode *get_field() const { return expr_.field; }
  ValueExprSqlNode *get_value() const { return expr_.value; }
  ComparisonExprSqlNode *get_comparison() const { return expr_.comparison; }
  ConjunctionExprSqlNode *get_conjunction() const { return expr_.conjunction; }
  ArithmeticExprSqlNode *get_arithmetic() const { return expr_.arithmetic; }
  NamedExprSqlNode *get_named() const { return expr_.named; }
  FunctionExprSqlNode *get_function() const { return expr_.function; }
  ContainExprSqlNode *get_contain() const { return expr_.contain; }
  ExistsExprSqlNode *get_exists() const { return expr_.exists; }
  LikeExprSqlNode *get_like() const { return expr_.like; }
  ListExprSqlNode *get_list() const { return expr_.list; }
  SetExprSqlNode *get_set() const { return expr_.set; }
  NullCheckExprSqlNode *get_null() const { return expr_.null; }
};

struct StarExprSqlNode {};

struct FieldExprSqlNode {
  std::string table_name;
  std::string field_name;
};

struct ValueExprSqlNode {
  Value value;
};

template <typename T> ExprSqlNode *get_expr_pointer(T *p) {
  if (p == nullptr)
    return nullptr;
  else if constexpr (std::is_same<T, ExprSqlNode>())
    return p;
  else
    return new ExprSqlNode(p);
}

struct ComparisonExprSqlNode {
  CompOp op;
  ExprSqlNode *left;
  ExprSqlNode *right;
  template <typename T1, typename T2>
  ComparisonExprSqlNode(CompOp op, T1 *left, T2 *right)
      : op(op), left(get_expr_pointer(left)), right(get_expr_pointer(right)) {}
  ~ComparisonExprSqlNode();
};

struct ConjunctionExprSqlNode {
  ConjunctionType type;
  ExprSqlNode *left;
  ExprSqlNode *right;
  template <typename T1, typename T2>
  ConjunctionExprSqlNode(ConjunctionType type, T1 *left, T2 *right)
      : type(type), left(get_expr_pointer(left)), right(get_expr_pointer(right)) {}
  ~ConjunctionExprSqlNode();
};

struct ArithmeticExprSqlNode {
  ArithmeticType type;
  ExprSqlNode *left;
  ExprSqlNode *right;
  template <typename T1, typename T2>
  ArithmeticExprSqlNode(ArithmeticType type, T1 *left, T2 *right)
      : type(type), left(get_expr_pointer(left)), right(get_expr_pointer(right)) {}
  ~ArithmeticExprSqlNode();
};

enum class NamedType {
  AGGREGATION,
  SUBQUERY,
};

struct NamedExprSqlNode {
  std::string name;
  union {
    AggregationExprSqlNode *aggr;
  } child;
  NamedType type;
  NamedExprSqlNode(std::string name, AggregationExprSqlNode *aggr) : name(name), type(NamedType::AGGREGATION) {
    child.aggr = aggr;
  }
  ~NamedExprSqlNode();
};

enum class AggregationType {
  AGGR_MAX,
  AGGR_MIN,
  AGGR_AVG,
  AGGR_SUM,
  AGGR_COUNT,
};

struct AggregationExprSqlNode {
  std::vector<ExprSqlNode *> children;
  AggregationType type;
  AggregationExprSqlNode(AggregationType type, std::vector<ExprSqlNode *> &children)
      : type(type), children(std::move(children)) {}
  AggregationExprSqlNode(AggregationType type) : type(type) {}
  ~AggregationExprSqlNode();
};

struct FunctionExprSqlNode {
  FunctionType type;
  std::vector<ExprSqlNode *> children;
  FunctionExprSqlNode(FunctionType type, std::vector<ExprSqlNode *> *child) : type(type), children(std::move(*child)) {}
  ~FunctionExprSqlNode();
};

struct ContainExprSqlNode {
  ContainType type;
  ExprSqlNode *left = nullptr;
  ExprSqlNode *right = nullptr;
  template <typename T1, typename T2>
  ContainExprSqlNode(ContainType type, T1 *left, T2 *right)
      : type(type), left(get_expr_pointer(left)), right(get_expr_pointer(right)) {}
  ~ContainExprSqlNode();
};

struct ExistsExprSqlNode {
  bool exists;
  ExprSqlNode *left = nullptr;
  template <typename T> ExistsExprSqlNode(bool exists, T *left) : exists(exists), left(get_expr_pointer(left)) {}
  ~ExistsExprSqlNode();
};

struct LikeExprSqlNode {
  bool like;
  ExprSqlNode *left = nullptr;
  std::string like_s;
  template <typename T1>
  LikeExprSqlNode(bool like, T1 *left, std::string like_s) : like(like), left(get_expr_pointer(left)), like_s(like_s) {}
  ~LikeExprSqlNode();
};

struct NullCheckExprSqlNode {
  bool is_null;
  ExprSqlNode *left = nullptr;
  template <typename T> NullCheckExprSqlNode(bool is_null, T *left) : is_null(is_null), left(get_expr_pointer(left)) {}
  ~NullCheckExprSqlNode();
};

struct ListExprSqlNode {
  SelectSqlNode *select = nullptr;
  ListExprSqlNode(SelectSqlNode *select) : select(select) {}
  ~ListExprSqlNode();
};

struct SetExprSqlNode {
  std::vector<ExprSqlNode *> expressions;
  SetExprSqlNode() = default;
  ~SetExprSqlNode() = default;
};

struct OrderBySqlNode {
  FieldExprSqlNode *field;
  Order order;
  ~OrderBySqlNode() {
    if (field)
      delete field;
  }
};

struct JoinSqlNode {
  std::string relation;                              ///< 查询的表
  std::string alias;                                 ///< 表别名
  JoinSqlNode *sub_join = nullptr;                   ///< 子join
  ConjunctionExprSqlNode *join_conditions = nullptr; ///< join条件
};

/**
 * @brief 描述一个select语句
 * @ingroup SQLParser
 * @details 一个正常的select语句描述起来比这个要复杂很多，这里做了简化。
 * 一个select语句由三部分组成，分别是select, from, where。
 * select部分表示要查询的字段，from部分表示要查询的表，where部分表示查询的条件。
 * 比如 from 中可以是多个表，也可以是另一个查询语句，这里仅仅支持表，也就是 relations。
 * where 条件 conditions，这里表示使用AND串联起来多个条件。正常的SQL语句会有OR，NOT等，
 * 甚至可以包含复杂的表达式。
 */

struct SelectAttribute {
  ExprSqlNode *expr;
  std::string alias;
};

struct SelectSqlNode {
  std::vector<SelectAttribute *> attributes;           ///< attributes in select clause
  std::vector<FieldExprSqlNode *> groupbys;            ///< groupbys
  std::vector<OrderBySqlNode *> orderbys;              ///< orderbys
  JoinSqlNode *tables = nullptr;                       ///< 查询的表，以及join条件
  ConjunctionExprSqlNode *conditions = nullptr;        ///< 查询条件，使用AND串联起来多个条件
  ConjunctionExprSqlNode *having_conditions = nullptr; ///< having查询条件
  std::string sql;
  ~SelectSqlNode() {
    for (auto *x : attributes)
      delete x;
    for (auto *x : groupbys)
      delete x;
    for (auto *x : orderbys)
      delete x;

    if (conditions)
      delete conditions;
    if (having_conditions)
      delete having_conditions;
    if (tables)
      delete tables;
  }
};

/**
 * @brief 算术表达式计算的语法树
 * @ingroup SQLParser
 */
struct CalcSqlNode {
  std::vector<ExprSqlNode *> expressions; ///< calc clause

  ~CalcSqlNode();
};

/**
 * @brief 描述一个insert语句
 * @ingroup SQLParser
 * @details 于Selects类似，也做了很多简化
 */
struct InsertSqlNode {
  std::string relation_name;                      ///< Relation to insert into
  std::vector<std::vector<ExprSqlNode *>> values; ///< 要插入的值
  std::vector<std::string> name_lists;
  ~InsertSqlNode() {
    for (auto &x : values)
      for (auto *y : x)
        delete y;
  }
};

/**
 * @brief 描述一个delete语句
 * @ingroup SQLParser
 */
struct DeleteSqlNode {
  std::string relation_name; ///< Relation to delete from
  ConjunctionExprSqlNode *conditions = nullptr;
  ~DeleteSqlNode() {
    if (conditions)
      delete conditions;
  }
};

struct UpdateSetSqlNode {
  std::string field_name;
  ExprSqlNode *expr;
};

/**
 * @brief 描述一个update语句
 * @ingroup SQLParser
 */
struct UpdateSqlNode {
  std::string relation_name;            ///< Relation to update
  std::vector<UpdateSetSqlNode *> sets; ///< sets
  ConjunctionExprSqlNode *conditions = nullptr;
  ~UpdateSqlNode() {
    if (conditions)
      delete conditions;
    for (auto x : sets)
      delete x;
  }
};

/**
 * @brief 描述一个属性
 * @ingroup SQLParser
 * @details 属性，或者说字段(column, field)
 * Rel -> Relation
 * Attr -> Attribute
 */
struct AttrInfoSqlNode {
  AttrType type;    ///< Type of attribute
  std::string name; ///< Attribute name
  size_t length;    ///< Length of attribute
  bool nullable;    ///< nullable
};

/**
 * @brief 描述一个create table语句
 * @ingroup SQLParser
 * @details 这里也做了很多简化。
 */
struct CreateTableSqlNode {
  std::string relation_name;               ///< Relation name
  std::vector<AttrInfoSqlNode> attr_infos; ///< attributes
  ParsedSqlNode *select = nullptr;
  ~CreateTableSqlNode();
};

struct CreateViewSqlNode {
  std::string view_name; ///< Relation name
  ParsedSqlNode *select = nullptr;
  std::string select_sql;
  std::vector<std::string> names; ///< attributes
  ~CreateViewSqlNode();
};

/**
 * @brief 描述一个drop table语句
 * @ingroup SQLParser
 */
struct DropTableSqlNode {
  std::string relation_name; ///< 要删除的表名
};

/**
 * @brief 描述一个create index语句
 * @ingroup SQLParser
 * @details 创建索引时，需要指定索引名，表名，字段名。
 * 正常的SQL语句中，一个索引可能包含了多个字段，这里仅支持一个字段。
 */
struct CreateIndexSqlNode {
  std::string index_name;                   ///< Index name
  std::string relation_name;                ///< Relation name
  std::vector<std::string> attribute_names; ///< Attribute name
  bool unique;                              ///< unique
};

/**
 * @brief 描述一个drop index语句
 * @ingroup SQLParser
 */
struct DropIndexSqlNode {
  std::string index_name;    ///< Index name
  std::string relation_name; ///< Relation name
};

/**
 * @brief 描述一个desc table语句
 * @ingroup SQLParser
 * @details desc table 是查询表结构信息的语句
 */
struct DescTableSqlNode {
  std::string relation_name;
};

/**
 * @brief 描述一个load data语句
 * @ingroup SQLParser
 * @details 从文件导入数据到表中。文件中的每一行就是一条数据，每行的数据类型、字段个数都与表保持一致
 */
struct LoadDataSqlNode {
  std::string relation_name;
  std::string file_name;
};

/**
 * @brief 设置变量的值
 * @ingroup SQLParser
 * @note 当前还没有查询变量
 */
struct SetVariableSqlNode {
  std::string name;
  Value value;
};

class ParsedSqlNode;

/**
 * @brief 描述一个explain语句
 * @ingroup SQLParser
 * @details 会创建operator的语句，才能用explain输出执行计划。
 * 一个command就是一个语句，比如select语句，insert语句等。
 * 可能改成SqlCommand更合适。
 */
struct ExplainSqlNode {
  std::unique_ptr<ParsedSqlNode> sql_node;
};

/**
 * @brief 解析SQL语句出现了错误
 * @ingroup SQLParser
 * @details 当前解析时并没有处理错误的行号和列号
 */
struct ErrorSqlNode {
  std::string error_msg;
  int line;
  int column;
};

struct ShowIndexSqlNode {
  std::string table_name;
};

/**
 * @brief 表示一个SQL语句的类型
 * @ingroup SQLParser
 */
enum SqlCommandFlag {
  SCF_ERROR = 0,
  SCF_CALC,
  SCF_SELECT,
  SCF_INSERT,
  SCF_UPDATE,
  SCF_DELETE,
  SCF_CREATE_TABLE,
  SCF_CREATE_VIEW,
  SCF_DROP_TABLE,
  SCF_CREATE_INDEX,
  SCF_DROP_INDEX,
  SCF_SYNC,
  SCF_SHOW_TABLES,
  SCF_SHOW_INDEX,
  SCF_DESC_TABLE,
  SCF_BEGIN, ///< 事务开始语句，可以在这里扩展只读事务
  SCF_COMMIT,
  SCF_CLOG_SYNC,
  SCF_ROLLBACK,
  SCF_LOAD_DATA,
  SCF_HELP,
  SCF_EXIT,
  SCF_EXPLAIN,
  SCF_SET_VARIABLE, ///< 设置变量
};
/**
 * @brief 表示一个SQL语句
 * @ingroup SQLParser
 */
class ParsedSqlNode {
public:
  enum SqlCommandFlag flag;
  union {
    ErrorSqlNode *error;
    CalcSqlNode *calc;
    SelectSqlNode *selection;
    InsertSqlNode *insertion;
    DeleteSqlNode *deletion;
    UpdateSqlNode *update;
    CreateTableSqlNode *create_table;
    CreateViewSqlNode *create_view;
    DropTableSqlNode *drop_table;
    CreateIndexSqlNode *create_index;
    DropIndexSqlNode *drop_index;
    DescTableSqlNode *desc_table;
    ShowIndexSqlNode *show_index;
    LoadDataSqlNode *load_data;
    ExplainSqlNode *explain;
    SetVariableSqlNode *set_variable;
  } node;

public:
  ParsedSqlNode();
  explicit ParsedSqlNode(SqlCommandFlag flag);
  ~ParsedSqlNode();
};

/**
 * @brief 表示语法解析后的数据
 * @ingroup SQLParser
 */
class ParsedSqlResult {
public:
  void add_sql_node(std::unique_ptr<ParsedSqlNode> sql_node);
  std::vector<std::unique_ptr<ParsedSqlNode>> &sql_nodes() { return sql_nodes_; }

private:
  std::vector<std::unique_ptr<ParsedSqlNode>> sql_nodes_; ///< 这里记录SQL命令。虽然看起来支持多个，但是当前仅处理一个
};
