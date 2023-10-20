/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    INDEX = 263,                   /* INDEX  */
    UNIQUE = 264,                  /* UNIQUE  */
    CALC = 265,                    /* CALC  */
    SELECT = 266,                  /* SELECT  */
    DESC = 267,                    /* DESC  */
    SHOW = 268,                    /* SHOW  */
    SYNC = 269,                    /* SYNC  */
    INSERT = 270,                  /* INSERT  */
    DELETE = 271,                  /* DELETE  */
    UPDATE = 272,                  /* UPDATE  */
    LBRACE = 273,                  /* LBRACE  */
    RBRACE = 274,                  /* RBRACE  */
    COMMA = 275,                   /* COMMA  */
    TRX_BEGIN = 276,               /* TRX_BEGIN  */
    TRX_COMMIT = 277,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 278,            /* TRX_ROLLBACK  */
    INT_T = 279,                   /* INT_T  */
    STRING_T = 280,                /* STRING_T  */
    FLOAT_T = 281,                 /* FLOAT_T  */
    DATE_T = 282,                  /* DATE_T  */
    TEXT_T = 283,                  /* TEXT_T  */
    HELP = 284,                    /* HELP  */
    EXIT = 285,                    /* EXIT  */
    DOT = 286,                     /* DOT  */
    INTO = 287,                    /* INTO  */
    VALUES = 288,                  /* VALUES  */
    FROM = 289,                    /* FROM  */
    WHERE = 290,                   /* WHERE  */
    AND = 291,                     /* AND  */
    SET = 292,                     /* SET  */
    ON = 293,                      /* ON  */
    LOAD = 294,                    /* LOAD  */
    DATA = 295,                    /* DATA  */
    INFILE = 296,                  /* INFILE  */
    EXPLAIN = 297,                 /* EXPLAIN  */
    EQ = 298,                      /* EQ  */
    LT = 299,                      /* LT  */
    GT = 300,                      /* GT  */
    LE = 301,                      /* LE  */
    GE = 302,                      /* GE  */
    NE = 303,                      /* NE  */
    MIN = 304,                     /* MIN  */
    MAX = 305,                     /* MAX  */
    AVG = 306,                     /* AVG  */
    SUM = 307,                     /* SUM  */
    COUNT = 308,                   /* COUNT  */
    GROUP = 309,                   /* GROUP  */
    ORDER = 310,                   /* ORDER  */
    BY = 311,                      /* BY  */
    ASC = 312,                     /* ASC  */
    HAVING = 313,                  /* HAVING  */
    LENGTH = 314,                  /* LENGTH  */
    ROUND = 315,                   /* ROUND  */
    DATE_FORMAT = 316,             /* DATE_FORMAT  */
    INNER = 317,                   /* INNER  */
    JOIN = 318,                    /* JOIN  */
    NOT = 319,                     /* NOT  */
    IN = 320,                      /* IN  */
    EXISTS = 321,                  /* EXISTS  */
    LIKE = 322,                    /* LIKE  */
    NULL_V = 323,                  /* NULL_V  */
    NULLABLE = 324,                /* NULLABLE  */
    IS = 325,                      /* IS  */
    AS = 326,                      /* AS  */
    VIEW = 327,                    /* VIEW  */
    NUMBER = 328,                  /* NUMBER  */
    FLOAT = 329,                   /* FLOAT  */
    ID = 330,                      /* ID  */
    SSS = 331,                     /* SSS  */
    OR = 332,                      /* OR  */
    UMINUS = 333                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 132 "yacc_sql.y"

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

#line 179 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
