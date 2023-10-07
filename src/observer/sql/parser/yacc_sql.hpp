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
    CALC = 264,                    /* CALC  */
    SELECT = 265,                  /* SELECT  */
    DESC = 266,                    /* DESC  */
    SHOW = 267,                    /* SHOW  */
    SYNC = 268,                    /* SYNC  */
    INSERT = 269,                  /* INSERT  */
    DELETE = 270,                  /* DELETE  */
    UPDATE = 271,                  /* UPDATE  */
    LBRACE = 272,                  /* LBRACE  */
    RBRACE = 273,                  /* RBRACE  */
    COMMA = 274,                   /* COMMA  */
    TRX_BEGIN = 275,               /* TRX_BEGIN  */
    TRX_COMMIT = 276,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 277,            /* TRX_ROLLBACK  */
    INT_T = 278,                   /* INT_T  */
    STRING_T = 279,                /* STRING_T  */
    FLOAT_T = 280,                 /* FLOAT_T  */
    DATE_T = 281,                  /* DATE_T  */
    TEXT_T = 282,                  /* TEXT_T  */
    HELP = 283,                    /* HELP  */
    EXIT = 284,                    /* EXIT  */
    DOT = 285,                     /* DOT  */
    INTO = 286,                    /* INTO  */
    VALUES = 287,                  /* VALUES  */
    FROM = 288,                    /* FROM  */
    WHERE = 289,                   /* WHERE  */
    AND = 290,                     /* AND  */
    SET = 291,                     /* SET  */
    ON = 292,                      /* ON  */
    LOAD = 293,                    /* LOAD  */
    DATA = 294,                    /* DATA  */
    INFILE = 295,                  /* INFILE  */
    EXPLAIN = 296,                 /* EXPLAIN  */
    EQ = 297,                      /* EQ  */
    LT = 298,                      /* LT  */
    GT = 299,                      /* GT  */
    LE = 300,                      /* LE  */
    GE = 301,                      /* GE  */
    NE = 302,                      /* NE  */
    MIN = 303,                     /* MIN  */
    MAX = 304,                     /* MAX  */
    AVG = 305,                     /* AVG  */
    COUNT = 306,                   /* COUNT  */
    GROUP = 307,                   /* GROUP  */
    ORDER = 308,                   /* ORDER  */
    BY = 309,                      /* BY  */
    ASC = 310,                     /* ASC  */
    HAVING = 311,                  /* HAVING  */
    LENGTH = 312,                  /* LENGTH  */
    ROUND = 313,                   /* ROUND  */
    DATE_FORMAT = 314,             /* DATE_FORMAT  */
    INNER = 315,                   /* INNER  */
    JOIN = 316,                    /* JOIN  */
    NOT = 317,                     /* NOT  */
    IN = 318,                      /* IN  */
    NULL_V = 319,                  /* NULL_V  */
    NULLABLE = 320,                /* NULLABLE  */
    NUMBER = 321,                  /* NUMBER  */
    FLOAT = 322,                   /* FLOAT  */
    ID = 323,                      /* ID  */
    SSS = 324,                     /* SSS  */
    OR = 325,                      /* OR  */
    UMINUS = 326                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 125 "yacc_sql.y"

  ParsedSqlNode *                               sql_node;
  ComparisonExprSqlNode *                       condition;
  ValueExprSqlNode *                            value_expr;
  Value *                                       value;
  enum CompOp                                   comp;
  ContainType                                   contain_op;
  ContainExprSqlNode *                          contain;
  ListExprSqlNode *                             list;
  AggregationType                               aggr;
  FunctionType                                  func;
  FieldExprSqlNode *                            rel_attr;
  std::vector<FieldExprSqlNode *> *             rel_attr_list;
  std::vector<AttrInfoSqlNode> *                attr_infos;
  AttrInfoSqlNode *                             attr_info;
  ExprSqlNode *                                 expression;
  std::vector<ExprSqlNode *> *                  expression_list;
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

#line 166 "yacc_sql.hpp"

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
