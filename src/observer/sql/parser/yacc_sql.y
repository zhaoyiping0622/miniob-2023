
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  ErrorSqlNode *error = new ErrorSqlNode;
  error->error_msg = msg;
  error->line = llocp->first_line;
  error->column = llocp->first_column;
  error_sql_node->node.error=error;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ExprSqlNode *create_arithmetic_expression(ArithmeticType type,
                                          ExprSqlNode *left,
                                          ExprSqlNode *right,
                                          const char *sql_string,
                                          YYLTYPE *llocp)
{
  ArithmeticExprSqlNode *expr = new ArithmeticExprSqlNode(type, left, right);
  ExprSqlNode *ret = new ExprSqlNode(expr);
  ret->set_name(token_name(sql_string, llocp));
  return ret;
}

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        UNIQUE
        CALC
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        EQ
        LT
        GT
        LE
        GE
        NE
        MIN
        MAX
        AVG
        SUM
        COUNT
        GROUP
        ORDER
        BY
        ASC
        HAVING
        LENGTH
        ROUND
        DATE_FORMAT
        INNER
        JOIN
        NOT
        IN
        EXISTS
        LIKE
        NULL_V
        NULLABLE
        IS
        AS
        VIEW

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                               sql_node;
  ComparisonExprSqlNode *                       condition;
  ValueExprSqlNode *                            value_expr;
  Value *                                       value;
  enum CompOp                                   comp;
  ContainType                                   contain_op;
  ContainExprSqlNode *                          contain;
  ExistsExprSqlNode *                           exists;
  ListExprSqlNode *                             list;
  SetExprSqlNode *                              set;
  AggregationType                               aggr;
  FunctionType                                  func;
  FieldExprSqlNode *                            rel_attr;
  std::vector<FieldExprSqlNode *> *             rel_attr_list;
  std::vector<AttrInfoSqlNode> *                attr_infos;
  AttrInfoSqlNode *                             attr_info;
  ExprSqlNode *                                 expression;
  UpdateSetSqlNode *                            update_set;
  std::vector<UpdateSetSqlNode *> *             update_set_list;
  std::vector<ExprSqlNode *> *                  expression_list;
  SelectAttribute *                             select_attr;
  std::vector<SelectAttribute *> *              select_attr_list;
  std::vector<std::vector<ExprSqlNode *>> *     record_list;
  ConjunctionExprSqlNode *                      conjunction;
  std::vector<std::string> *                    relation_list;
  std::vector<std::string> *                    id_list;
  JoinSqlNode *                                 join;
  OrderBySqlNode *                              order_unit;
  std::vector<OrderBySqlNode *> *               order_unit_list;
  Order                                         order;
  char *                                        string;
  int                                           number;
  float                                         floats;
  bool                                          bools;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <condition>           condition
%type <value>               value
%type <value_expr>          value_expr
%type <number>              number
%type <comp>                comp_op
%type <aggr>                aggr_op
%type <func>                func_op
%type <contain_op>          contain_op;
%type <rel_attr>            rel_attr
%type <rel_attr_list>       rel_attr_list
%type <rel_attr_list>       groupby
%type <attr_infos>          attr_def_list
%type <attr_infos>          attr_list
%type <attr_info>           attr_def
%type <expression_list>     record
%type <record_list>         record_list
%type <conjunction>         where
%type <conjunction>         having
%type <conjunction>         conjunction
%type <conjunction>         joined_on
%type <contain>             contain
%type <exists>              exists
%type <list>                list_expr
%type <set>                 set_expr
%type <select_attr>         select_attr
%type <select_attr_list>    select_attr_list
%type <join>                rel_list
%type <join>                from
%type <join>                joined_tables
%type <join>                joined_tables_inner
%type <expression>          expression
%type <expression_list>     expression_list
%type <expression_list>     expression_list_empty
%type <order_unit_list>     order_unit_list
%type <order_unit_list>     orderby
%type <update_set>          update_set
%type <update_set_list>     update_set_list
%type <id_list>             ids
%type <id_list>             brace_id_list
%type <order_unit>          order_unit
%type <order>               order
%type <bools>               null_def
%type <bools>               null_check
%type <bools>               unique
%type <bools>               like_op
%type <bools>               exists_op
%type <string>              as_info
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            as_select
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            create_view_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            show_index_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
%type <string>              non_reserve
%type <string>              id
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left AS
%left OR
%left AND
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | create_view_stmt
  | drop_table_stmt
  | show_tables_stmt
  | show_index_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE id {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      auto *drop_table = new DropTableSqlNode;
      $$->node.drop_table = drop_table;
      drop_table->relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC id  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      auto *desc_table = new DescTableSqlNode;
      $$->node.desc_table = desc_table;
      desc_table->relation_name = $2;
      free($2);
    }
    ;

show_index_stmt:
    SHOW INDEX FROM id {
      $$ = new ParsedSqlNode(SCF_SHOW_INDEX);
      auto *show_index = new ShowIndexSqlNode;
      $$->node.show_index = show_index;
      show_index->table_name = $4;
      free($4);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE unique INDEX id ON id LBRACE id ids RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode *create_index = new CreateIndexSqlNode;
      $$->node.create_index = create_index;
      create_index->unique = $2;
      create_index->index_name = $4;
      create_index->relation_name = $6;
      $9->push_back($8);
      create_index->attribute_names.swap(*$9);
      delete $9;
      std::reverse(create_index->attribute_names.begin(), create_index->attribute_names.end());
      free($4);
      free($6);
      free($8);
    }
    ;

unique:
    {
      $$ = false;
    }
    | UNIQUE {
      $$ = true;
    }

ids:
   {
      $$ = new std::vector<std::string>();
   }
   | COMMA id ids {
      $3->push_back($2);
      free($2);
      $$ = $3;
   }

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX id ON id
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      auto *drop_index = new DropIndexSqlNode;
      $$->node.drop_index = drop_index;
      drop_index->index_name = $3;
      drop_index->relation_name = $5;
      free($3);
      free($5);
    }
    ;

create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE id attr_list as_select
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode *create_table = new CreateTableSqlNode;
      $$->node.create_table = create_table;
      create_table->relation_name = $3;
      free($3);
      if($4 != nullptr) {
        create_table->attr_infos.swap(*$4);
        delete $4;
      }
      create_table->select = $5;
    }
    ;

create_view_stmt:
    CREATE VIEW id brace_id_list AS select_stmt
    {
      $$ = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode *create_view = new CreateViewSqlNode;
      $$->node.create_view = create_view;
      create_view->view_name = $3;
      free($3);
      if($4 != nullptr) {
        create_view->names.swap(*$4);
        delete $4;
      }
      create_view->select = $6;
      create_view->select_sql = $6->node.selection->sql;
    }

brace_id_list:
    {
      $$ = nullptr;
    }
    | LBRACE id ids RBRACE {
      if ($3 == nullptr) {
        $$ = new std::vector<std::string>();
      } else {
        $$ = $3;
      }
      $$->push_back($2);
      free($2);
      std::reverse($$->begin(), $$->end());
    }

attr_list:
    {
      $$ = nullptr;
    }
    | LBRACE attr_def attr_def_list RBRACE {
      if ($3 == nullptr) {
        $$ = new std::vector<AttrInfoSqlNode>;
      } else {
        $$ = $3;
      }
      $$->emplace_back(*$2);
      std::reverse($$->begin(), $$->end());
    }

as_select:
    {
      $$ = nullptr;
    }
    | AS select_stmt {
      $$ = $2;
    }
    | select_stmt {
      $$ = $1;
    }

attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    id type LBRACE number RBRACE null_def
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = $6;
      free($1);
    }
    | id type null_def
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = $3;
      free($1);
    }
    ;

null_def:
    {
      $$ = true;
    }
    | NOT NULL_V {
      $$ = false;
    }
    | NULL_V {
      $$ = true;
    }
    | NULLABLE {
      $$ = true;
    }

number:
    NUMBER {$$ = $1;}
    ;

type:
    INT_T      { $$=INTS; }
    | STRING_T { $$=CHARS; }
    | FLOAT_T  { $$=FLOATS; }
    | DATE_T   { $$=DATES; }
    | TEXT_T   { $$=TEXTS; }
    ;

insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO id brace_id_list VALUES record record_list
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      auto *insertion = new InsertSqlNode;
      $$->node.insertion = insertion;
      insertion->relation_name = $3;
      if ($7 != nullptr) {
        insertion->values.swap(*$7);
        delete $7;
      }
      insertion->values.emplace_back(*$6);
      delete $6;
      std::reverse(insertion->values.begin(), insertion->values.end());
      free($3);
      if($4 != nullptr) {
        insertion->name_lists.swap(*$4);
        delete $4;
      }
    }
    ;

record_list:
    {
      $$ = nullptr;
    }
    | COMMA record record_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::vector<ExprSqlNode *>>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }

record:
    LBRACE expression_list_empty RBRACE
    {
      if ($2 != nullptr) {
        $$ = $2;
      } else {
        $$ = new std::vector<ExprSqlNode *>;
      }
      reverse($$->begin(), $$->end());
    }

value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
    }
    | NULL_V {
      $$ = new Value;
      $$->set_null();
    }
    ;
    
value_expr:
    value {
      $$ = new ValueExprSqlNode;
      $$->value = *$1;
      delete $1;
    }

delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM id where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      auto *deletion = new DeleteSqlNode;
      $$->node.deletion = deletion;
      deletion->relation_name = $3;
      deletion->conditions = $4;
      free($3);
    }
    ;

update_stmt:      /*  update 语句的语法解析树*/
    UPDATE id SET update_set_list where 
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      auto *update = new UpdateSqlNode;
      $$->node.update = update;
      update->relation_name = $2;
      update->sets.swap(*$4);
      delete $4;
      update->conditions = $5;
      free($2);
    }
    ;

update_set_list:
    update_set 
    {
      $$ = new std::vector<UpdateSetSqlNode *>(1, $1);
    }
    | update_set COMMA update_set_list {
      $$ = $3;
      $$->push_back($1);
    }

update_set:
    id EQ expression {
      $$ = new UpdateSetSqlNode;
      $$->field_name = $1;
      free($1);
      $$->expr = $3;
    }

select_stmt:        /*  select 语句的语法解析树*/
    SELECT select_attr_list from where groupby having orderby
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      auto* selection = new SelectSqlNode;
      $$->node.selection = selection;
      if ($2 != nullptr) {
        std::reverse($2->begin(), $2->end());
        selection->attributes.swap(*$2);
        delete $2;
      }
      if ($3 != nullptr) {
        selection->tables=$3;
      }
      if ($5 != nullptr) {
        selection->groupbys.swap(*$5);
        delete $5;
      }
      if ($7 != nullptr) {
        selection->orderbys.swap(*$7);
        delete $7;
      }

      selection->conditions = $4;
      selection->having_conditions=$6;
      selection->sql = token_name(sql_string, &@$);
    }
    ;

from:
    {
      $$ = nullptr;
    }
    | FROM rel_list {
      $$ = $2;
    }
    | FROM joined_tables {
      $$ = $2;
    }
    ;

joined_tables:
    joined_tables_inner INNER JOIN id as_info joined_on {
      $$ = new JoinSqlNode;
      $$->relation=$4;
      free($4);
      $$->alias = $5;
      if(*$5) free($5);
      $$->sub_join=$1;
      $$->join_conditions=$6;  
    }

joined_tables_inner:
    id {
      $$ = new JoinSqlNode;
      $$->relation = $1;
      free($1);
    }
    | joined_tables_inner INNER JOIN id as_info joined_on {
      $$ = new JoinSqlNode;
      $$->relation=$4;
      free($4);
      $$->sub_join=$1;
      $$->alias=$5;
      if(*$5) free($5);
      $$->join_conditions=$6;  
    }

joined_on:
    ON conjunction {
      $$ = $2;
    }

having:
    {
      $$ = nullptr;
    }
    | HAVING conjunction {
      $$ = $2;
    }
    ;

groupby:
    {
      $$ = nullptr;
    }
    | GROUP BY rel_attr rel_attr_list 
    {
      $$ = $4;
      if ($$ == nullptr) {
        $$ = new std::vector<FieldExprSqlNode *>();
      }
      $$->push_back($3);
      std::reverse($$->begin(), $$->end());
    }

orderby:
    {
      $$ = nullptr;
    }
    | ORDER BY order_unit_list {
      $$ = $3;
      std::reverse($$->begin(), $$->end());
    }

order_unit_list:
    order_unit 
    {
      $$ = new std::vector<OrderBySqlNode *>();
      $$->push_back($1);
    }
    | order_unit COMMA order_unit_list 
    {
      $$ = $3;
      $$->push_back($1);
    }

order_unit:
    rel_attr order {
      $$ = new OrderBySqlNode;
      $$->field = $1;
      $$->order = $2;
    }

order:
    {
      $$ = Order::ASC;
    }
    | ASC {
      $$ = Order::ASC;
    }
    | DESC {
      $$ = Order::DESC;
    }

rel_attr_list:
    {
      $$ = nullptr;
    }
    | COMMA rel_attr rel_attr_list
    {
      $$ = $3;
      if ($$ == nullptr) {
        $$ = new std::vector<FieldExprSqlNode *>();
      }
      $$->push_back($2);
    }

calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      auto *tmp = new CalcSqlNode;
      $$->node.calc = tmp;
      std::reverse($2->begin(), $2->end());
      tmp->expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression
    {
      $$ = new std::vector<ExprSqlNode *>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<ExprSqlNode *>;
      }
      $$->emplace_back($1);
    }
    ;

expression_list_empty:
    {
      $$ = nullptr;
    }
    | expression_list {
      $$ = $1;
    }

expression:
    expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticType::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticType::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticType::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticType::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticType::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | '*' {
      $$ = new ExprSqlNode(new StarExprSqlNode);
    }
    | rel_attr {
      $$ = new ExprSqlNode($1);
      $$->set_name(token_name(sql_string, &@$));
    }
    | value_expr {
      $$ = new ExprSqlNode($1);
      $$->set_name(token_name(sql_string, &@$));
    }
    | aggr_op LBRACE expression_list_empty RBRACE {
      std::string name = token_name(sql_string, &@$);
      if ($3) {
        $$ = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode($1, *$3)));
      } else {
        $$ = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode($1)));
      }
      $$->set_name(name);
    }
    | func_op LBRACE expression_list_empty RBRACE {
      std::string name = token_name(sql_string, &@$);
      reverse($3->begin(), $3->end());
      $$ = new ExprSqlNode(new FunctionExprSqlNode($1, $3));
      delete $3;
      $$->set_name(name);
    }
    | list_expr {
      $$ = new ExprSqlNode($1);
      std::string name = token_name(sql_string, &@$);
      $$->set_name(name);
    }
    | set_expr {
      $$ = new ExprSqlNode($1);
      std::string name = token_name(sql_string, &@$);
      $$->set_name(name);
    }
    ;

select_attr_list:
    select_attr {
      $$ = new std::vector<SelectAttribute *>(1, $1);
    }
    | select_attr COMMA select_attr_list {
      $3->push_back($1);
      $$ = $3;
    }
    ;

select_attr:
    expression as_info {
      $$ = new SelectAttribute;
      $$->expr = $1;
      $$->alias = $2;
      if(*$2) free($2);
    }

as_info:
    {
      $$ = "";
    }
    | id {
      $$ = $1;
    }
    | AS id {
      $$ = $2;
    }

list_expr:
    LBRACE select_stmt RBRACE {
      $$ = new ListExprSqlNode($2->node.selection);
      $2->node.selection = nullptr;
      delete $2;
    }
    ;

set_expr:
    LBRACE expression COMMA expression_list RBRACE {
      $4->push_back($2);
      $$ = new SetExprSqlNode();
      $$->expressions.swap(*$4);
      delete $4;
    }
    ;

rel_attr:
    id {
      $$ = new FieldExprSqlNode;
      $$->field_name = $1;
      free($1);
    }
    | id DOT id {
      $$ = new FieldExprSqlNode;
      $$->table_name  = $1;
      $$->field_name = $3;
      free($1);
      free($3);
    }
    | id DOT '*' {
      $$ = new FieldExprSqlNode;
      $$->table_name  = $1;
      $$->field_name = "*";
      free($1);
    }
    ;

rel_list:
    /* empty */
    id as_info {
      $$ = new JoinSqlNode;
      $$->relation = $1;
      $$->alias = $2;
      if(*$2) free($2);
      free($1);
    }
    | rel_list COMMA id as_info {
      $$ = new JoinSqlNode;
      $$->relation = $3;
      free($3);
      $$->alias = $4;
      if(*$4) free($4);
      $$->sub_join = $1;
    }
    ;

where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE conjunction {
      $$ = $2;  
    }
    ;
conjunction:
    /* empty */
    {
      $$ = nullptr;
    }
    | contain {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, $1, static_cast<ExprSqlNode *>(nullptr));
    }
    | condition {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, $1, static_cast<ExprSqlNode *>(nullptr));
    }
    | expression like_op SSS {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new LikeExprSqlNode($2, $1, $3), static_cast<ExprSqlNode *>(nullptr));
      free($3);
    }
    | exists {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, $1, static_cast<ExprSqlNode *>(nullptr));
    }
    | expression null_check {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new NullCheckExprSqlNode($2, $1), static_cast<ExprSqlNode *>(nullptr));
    }
    | conjunction AND conjunction {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::AND, $1, $3);
    }
    | conjunction OR conjunction {
      $$ = new ConjunctionExprSqlNode(ConjunctionType::OR, $1, $3);
    }
    ;

null_check: 
    IS NULL_V {
      $$ = true;
    }
    | IS NOT NULL_V {
      $$ = false;
    }

condition:
    expression comp_op expression {
      $$ = new ComparisonExprSqlNode($2, $1, $3); 
    }
    ;

contain:
    expression contain_op expression {
      $$ = new ContainExprSqlNode($2, $1, $3);
    }
    ;

exists:
    exists_op expression {
      $$ = new ExistsExprSqlNode($1, $2);
    }

exists_op:
    EXISTS {
      $$ = true;
    }
    | NOT EXISTS {
      $$ = false;
    }

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    ;

contain_op:
      IN { $$ = ContainType::IN; }
    | NOT IN { $$ = ContainType::NOT_IN; }

like_op:
      LIKE { $$ = true; }
    | NOT LIKE { $$ = false; }

aggr_op:
      MIN { $$ = AggregationType::AGGR_MIN; }
    | MAX { $$ = AggregationType::AGGR_MAX; }
    | AVG { $$ = AggregationType::AGGR_AVG; }
    | SUM { $$ = AggregationType::AGGR_SUM; }
    | COUNT { $$ = AggregationType::AGGR_COUNT; }

func_op:
      LENGTH { $$ = FunctionType::LENGTH; }
    | ROUND { $$ = FunctionType::ROUND; }
    | DATE_FORMAT { $$ = FunctionType::DATE_FORMAT; }

load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE id 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      auto *load_data = new LoadDataSqlNode;
      $$->node.load_data = load_data;
      load_data->relation_name = $7;
      load_data->file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->node.explain = new ExplainSqlNode;
      $$->node.explain->sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET id EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      auto *set_variable = new SetVariableSqlNode;
      $$->node.set_variable = set_variable;
      set_variable->name  = $2;
      set_variable->value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;

id:
    non_reserve {
      $$ = $1;
    }
    | ID {
      $$ = $1;
    }

non_reserve:
    TABLES {
      $$ = strdup("tables");
    }
    | HELP {
      $$ = strdup("help");
    }
    | DATA {
      $$ = strdup("data");
    }
    | MIN {
      $$ = strdup("min");
    }
    | MAX {
      $$ = strdup("max");
    }
    | AVG {
      $$ = strdup("avg");
    }
    | SUM {
      $$ = strdup("sum");
    }
    | COUNT {
      $$ = strdup("count");
    }
    | NULLABLE {
      $$ = strdup("nullable");
    }

%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
