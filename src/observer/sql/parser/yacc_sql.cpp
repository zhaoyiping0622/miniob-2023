/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


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


#line 118 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_UNIQUE = 9,                     /* UNIQUE  */
  YYSYMBOL_CALC = 10,                      /* CALC  */
  YYSYMBOL_SELECT = 11,                    /* SELECT  */
  YYSYMBOL_DESC = 12,                      /* DESC  */
  YYSYMBOL_SHOW = 13,                      /* SHOW  */
  YYSYMBOL_SYNC = 14,                      /* SYNC  */
  YYSYMBOL_INSERT = 15,                    /* INSERT  */
  YYSYMBOL_DELETE = 16,                    /* DELETE  */
  YYSYMBOL_UPDATE = 17,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 18,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 19,                    /* RBRACE  */
  YYSYMBOL_COMMA = 20,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 21,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 22,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 23,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 24,                     /* INT_T  */
  YYSYMBOL_STRING_T = 25,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 26,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 27,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 28,                    /* TEXT_T  */
  YYSYMBOL_HELP = 29,                      /* HELP  */
  YYSYMBOL_EXIT = 30,                      /* EXIT  */
  YYSYMBOL_DOT = 31,                       /* DOT  */
  YYSYMBOL_INTO = 32,                      /* INTO  */
  YYSYMBOL_VALUES = 33,                    /* VALUES  */
  YYSYMBOL_FROM = 34,                      /* FROM  */
  YYSYMBOL_WHERE = 35,                     /* WHERE  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_SET = 37,                       /* SET  */
  YYSYMBOL_ON = 38,                        /* ON  */
  YYSYMBOL_LOAD = 39,                      /* LOAD  */
  YYSYMBOL_DATA = 40,                      /* DATA  */
  YYSYMBOL_INFILE = 41,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 42,                   /* EXPLAIN  */
  YYSYMBOL_EQ = 43,                        /* EQ  */
  YYSYMBOL_LT = 44,                        /* LT  */
  YYSYMBOL_GT = 45,                        /* GT  */
  YYSYMBOL_LE = 46,                        /* LE  */
  YYSYMBOL_GE = 47,                        /* GE  */
  YYSYMBOL_NE = 48,                        /* NE  */
  YYSYMBOL_MIN = 49,                       /* MIN  */
  YYSYMBOL_MAX = 50,                       /* MAX  */
  YYSYMBOL_AVG = 51,                       /* AVG  */
  YYSYMBOL_SUM = 52,                       /* SUM  */
  YYSYMBOL_COUNT = 53,                     /* COUNT  */
  YYSYMBOL_GROUP = 54,                     /* GROUP  */
  YYSYMBOL_ORDER = 55,                     /* ORDER  */
  YYSYMBOL_BY = 56,                        /* BY  */
  YYSYMBOL_ASC = 57,                       /* ASC  */
  YYSYMBOL_HAVING = 58,                    /* HAVING  */
  YYSYMBOL_LENGTH = 59,                    /* LENGTH  */
  YYSYMBOL_ROUND = 60,                     /* ROUND  */
  YYSYMBOL_DATE_FORMAT = 61,               /* DATE_FORMAT  */
  YYSYMBOL_INNER = 62,                     /* INNER  */
  YYSYMBOL_JOIN = 63,                      /* JOIN  */
  YYSYMBOL_NOT = 64,                       /* NOT  */
  YYSYMBOL_IN = 65,                        /* IN  */
  YYSYMBOL_EXISTS = 66,                    /* EXISTS  */
  YYSYMBOL_LIKE = 67,                      /* LIKE  */
  YYSYMBOL_NULL_V = 68,                    /* NULL_V  */
  YYSYMBOL_IS = 69,                        /* IS  */
  YYSYMBOL_AS = 70,                        /* AS  */
  YYSYMBOL_VIEW = 71,                      /* VIEW  */
  YYSYMBOL_NUMBER = 72,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 73,                     /* FLOAT  */
  YYSYMBOL_ID = 74,                        /* ID  */
  YYSYMBOL_SSS = 75,                       /* SSS  */
  YYSYMBOL_OR = 76,                        /* OR  */
  YYSYMBOL_77_ = 77,                       /* '+'  */
  YYSYMBOL_78_ = 78,                       /* '-'  */
  YYSYMBOL_79_ = 79,                       /* '*'  */
  YYSYMBOL_80_ = 80,                       /* '/'  */
  YYSYMBOL_UMINUS = 81,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 82,                  /* $accept  */
  YYSYMBOL_commands = 83,                  /* commands  */
  YYSYMBOL_command_wrapper = 84,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 85,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 86,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 87,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 88,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 89,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 90,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 91,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 92,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 93,           /* desc_table_stmt  */
  YYSYMBOL_show_index_stmt = 94,           /* show_index_stmt  */
  YYSYMBOL_create_index_stmt = 95,         /* create_index_stmt  */
  YYSYMBOL_unique = 96,                    /* unique  */
  YYSYMBOL_ids = 97,                       /* ids  */
  YYSYMBOL_drop_index_stmt = 98,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 99,         /* create_table_stmt  */
  YYSYMBOL_create_view_stmt = 100,         /* create_view_stmt  */
  YYSYMBOL_attr_list = 101,                /* attr_list  */
  YYSYMBOL_as_select = 102,                /* as_select  */
  YYSYMBOL_attr_def_list = 103,            /* attr_def_list  */
  YYSYMBOL_attr_def = 104,                 /* attr_def  */
  YYSYMBOL_null_def = 105,                 /* null_def  */
  YYSYMBOL_number = 106,                   /* number  */
  YYSYMBOL_type = 107,                     /* type  */
  YYSYMBOL_insert_stmt = 108,              /* insert_stmt  */
  YYSYMBOL_record_list = 109,              /* record_list  */
  YYSYMBOL_record = 110,                   /* record  */
  YYSYMBOL_value = 111,                    /* value  */
  YYSYMBOL_value_expr = 112,               /* value_expr  */
  YYSYMBOL_delete_stmt = 113,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 114,              /* update_stmt  */
  YYSYMBOL_update_set_list = 115,          /* update_set_list  */
  YYSYMBOL_update_set = 116,               /* update_set  */
  YYSYMBOL_select_stmt = 117,              /* select_stmt  */
  YYSYMBOL_from = 118,                     /* from  */
  YYSYMBOL_joined_tables = 119,            /* joined_tables  */
  YYSYMBOL_joined_tables_inner = 120,      /* joined_tables_inner  */
  YYSYMBOL_joined_on = 121,                /* joined_on  */
  YYSYMBOL_having = 122,                   /* having  */
  YYSYMBOL_groupby = 123,                  /* groupby  */
  YYSYMBOL_orderby = 124,                  /* orderby  */
  YYSYMBOL_order_unit_list = 125,          /* order_unit_list  */
  YYSYMBOL_order_unit = 126,               /* order_unit  */
  YYSYMBOL_order = 127,                    /* order  */
  YYSYMBOL_rel_attr_list = 128,            /* rel_attr_list  */
  YYSYMBOL_calc_stmt = 129,                /* calc_stmt  */
  YYSYMBOL_expression_list = 130,          /* expression_list  */
  YYSYMBOL_expression_list_empty = 131,    /* expression_list_empty  */
  YYSYMBOL_expression = 132,               /* expression  */
  YYSYMBOL_select_attr_list = 133,         /* select_attr_list  */
  YYSYMBOL_select_attr = 134,              /* select_attr  */
  YYSYMBOL_as_info = 135,                  /* as_info  */
  YYSYMBOL_list_expr = 136,                /* list_expr  */
  YYSYMBOL_set_expr = 137,                 /* set_expr  */
  YYSYMBOL_rel_attr = 138,                 /* rel_attr  */
  YYSYMBOL_rel_list = 139,                 /* rel_list  */
  YYSYMBOL_where = 140,                    /* where  */
  YYSYMBOL_conjunction = 141,              /* conjunction  */
  YYSYMBOL_null_check = 142,               /* null_check  */
  YYSYMBOL_condition = 143,                /* condition  */
  YYSYMBOL_contain = 144,                  /* contain  */
  YYSYMBOL_exists = 145,                   /* exists  */
  YYSYMBOL_exists_op = 146,                /* exists_op  */
  YYSYMBOL_comp_op = 147,                  /* comp_op  */
  YYSYMBOL_contain_op = 148,               /* contain_op  */
  YYSYMBOL_like_op = 149,                  /* like_op  */
  YYSYMBOL_aggr_op = 150,                  /* aggr_op  */
  YYSYMBOL_func_op = 151,                  /* func_op  */
  YYSYMBOL_load_data_stmt = 152,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 153,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 154,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 155,            /* opt_semicolon  */
  YYSYMBOL_id = 156,                       /* id  */
  YYSYMBOL_non_reserve = 157               /* non_reserve  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  96
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   420

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  76
/* YYNRULES -- Number of rules.  */
#define YYNRULES  177
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  287

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   332


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    79,    77,     2,    78,     2,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    81
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   259,   259,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   292,   298,   303,   309,   315,
     321,   327,   336,   342,   352,   362,   381,   384,   389,   392,
     399,   412,   428,   440,   443,   454,   457,   460,   466,   469,
     482,   491,   503,   506,   509,   514,   518,   519,   520,   521,
     522,   526,   544,   547,   558,   569,   573,   577,   582,   589,
     596,   608,   622,   626,   632,   640,   669,   672,   675,   681,
     692,   697,   708,   713,   716,   722,   725,   736,   739,   745,
     750,   757,   764,   767,   770,   775,   778,   788,   800,   805,
     817,   820,   825,   828,   831,   834,   837,   841,   844,   847,
     851,   855,   864,   871,   876,   884,   887,   894,   902,   905,
     908,   913,   921,   930,   935,   942,   952,   959,   971,   974,
     980,   983,   986,   989,   993,   996,   999,  1002,  1008,  1011,
    1016,  1022,  1028,  1033,  1036,  1041,  1042,  1043,  1044,  1045,
    1046,  1050,  1051,  1054,  1055,  1058,  1059,  1060,  1061,  1062,
    1065,  1066,  1067,  1070,  1085,  1094,  1106,  1107,  1111,  1114,
    1119,  1122,  1125,  1128,  1131,  1134,  1137,  1140
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "UNIQUE", "CALC", "SELECT", "DESC",
  "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "FLOAT_T", "DATE_T", "TEXT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "EQ", "LT", "GT", "LE", "GE", "NE", "MIN", "MAX", "AVG", "SUM", "COUNT",
  "GROUP", "ORDER", "BY", "ASC", "HAVING", "LENGTH", "ROUND",
  "DATE_FORMAT", "INNER", "JOIN", "NOT", "IN", "EXISTS", "LIKE", "NULL_V",
  "IS", "AS", "VIEW", "NUMBER", "FLOAT", "ID", "SSS", "OR", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "$accept", "commands", "command_wrapper",
  "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt",
  "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "show_index_stmt", "create_index_stmt", "unique",
  "ids", "drop_index_stmt", "create_table_stmt", "create_view_stmt",
  "attr_list", "as_select", "attr_def_list", "attr_def", "null_def",
  "number", "type", "insert_stmt", "record_list", "record", "value",
  "value_expr", "delete_stmt", "update_stmt", "update_set_list",
  "update_set", "select_stmt", "from", "joined_tables",
  "joined_tables_inner", "joined_on", "having", "groupby", "orderby",
  "order_unit_list", "order_unit", "order", "rel_attr_list", "calc_stmt",
  "expression_list", "expression_list_empty", "expression",
  "select_attr_list", "select_attr", "as_info", "list_expr", "set_expr",
  "rel_attr", "rel_list", "where", "conjunction", "null_check",
  "condition", "contain", "exists", "exists_op", "comp_op", "contain_op",
  "like_op", "aggr_op", "func_op", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", "id", "non_reserve", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-200)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-160)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     352,     6,    80,   265,   265,   119,    25,  -200,   -30,   -26,
     119,  -200,  -200,  -200,  -200,  -200,   119,   -23,   352,    14,
      24,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,   119,  -200,   -54,    29,   119,   119,  -200,
     229,  -200,  -200,    31,    38,    42,    43,    47,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,   265,  -200,  -200,  -200,
    -200,    -9,  -200,  -200,  -200,    57,    62,    19,  -200,    60,
      49,    67,  -200,  -200,  -200,  -200,  -200,  -200,  -200,    64,
     119,   119,    65,    74,    81,  -200,  -200,  -200,  -200,    85,
      53,   119,  -200,    87,   109,    16,  -200,   265,   265,   265,
     265,   265,   265,   265,     2,   119,  -200,  -200,   119,    94,
     265,   119,    99,    94,   119,   -34,    58,   119,     8,   125,
     103,   119,  -200,  -200,   265,  -200,   -35,   -35,  -200,  -200,
    -200,   124,   126,  -200,  -200,  -200,  -200,    82,   127,   113,
     182,    96,  -200,  -200,   133,  -200,    94,   132,   117,  -200,
     129,   154,   130,   125,  -200,  -200,  -200,   119,  -200,   157,
    -200,  -200,   114,   119,  -200,   118,  -200,   340,   -18,  -200,
    -200,  -200,   265,   134,   128,   265,   172,  -200,   119,   265,
     188,   119,   176,  -200,  -200,  -200,  -200,  -200,    -5,  -200,
     178,  -200,   119,   302,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,    32,  -200,  -200,   -40,  -200,   265,   265,   122,   182,
     182,   102,   119,   182,   143,   180,   133,  -200,  -200,   102,
     119,   154,  -200,   135,   136,  -200,  -200,   119,   302,  -200,
    -200,  -200,   137,  -200,   102,   102,  -200,  -200,   165,   186,
     -18,   152,  -200,  -200,   172,  -200,  -200,  -200,   190,  -200,
     194,   179,  -200,   119,  -200,   119,  -200,   -38,   119,   191,
     182,   156,   186,  -200,   200,    17,  -200,   194,  -200,   -18,
    -200,   119,  -200,  -200,  -200,  -200,  -200
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    36,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,    29,    30,    26,    25,     0,     0,     0,     0,
     166,    24,    23,    16,    17,    18,    19,    10,    11,    13,
      12,    14,    15,     8,     9,     5,     7,     6,     4,     3,
      20,    21,    22,     0,    37,     0,     0,     0,     0,   170,
       0,   171,   172,   173,   174,   175,   176,   177,   160,   161,
     162,    68,    65,    66,   169,    67,     0,   108,    69,   110,
      97,    98,   113,   114,   109,     0,     0,   123,   168,   118,
      76,   115,   173,   174,   175,   176,   177,    33,    32,     0,
       0,     0,     0,     0,     0,   164,     1,   167,     2,    43,
       0,     0,    31,     0,     0,     0,   107,     0,     0,     0,
       0,     0,   100,   100,     0,     0,   117,   119,     0,   128,
       0,     0,     0,   128,     0,     0,     0,     0,    45,     0,
       0,     0,   121,   106,     0,    99,   102,   103,   104,   105,
     101,     0,     0,   125,   124,   120,    78,     0,    77,   118,
     130,    85,   116,    34,     0,    70,   128,    72,     0,   165,
       0,    48,     0,     0,    41,    47,    42,     0,    40,     0,
     111,   112,     0,     0,   126,     0,   143,     0,   129,   132,
     131,   134,     0,     0,    83,   100,    62,    71,     0,     0,
       0,     0,     0,    56,    57,    58,    59,    60,    52,    46,
       0,   122,     0,   118,   144,   145,   146,   147,   148,   149,
     150,     0,   151,   153,     0,   135,     0,     0,     0,   130,
     130,   142,     0,   130,    87,     0,     0,    61,    73,    74,
       0,    48,    44,     0,     0,    54,    51,     0,   118,   127,
     152,   154,     0,   138,   140,   141,   133,   136,   137,    95,
      84,     0,    75,    64,    62,   163,    49,    55,     0,    53,
      38,     0,   139,     0,    86,     0,    63,    52,     0,     0,
     130,    79,    95,    88,    89,    92,    50,    38,    35,    82,
      96,     0,    94,    93,    91,    39,    90
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -200,  -200,   203,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,   -53,  -200,  -200,  -200,  -200,
    -200,    -8,    34,   -41,  -200,  -200,  -200,   -27,     3,   120,
    -200,  -200,  -200,    40,  -200,   -44,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,   -42,  -200,  -200,   -28,  -200,     1,  -112,
      -4,   131,  -200,  -146,  -200,  -200,  -199,  -200,  -116,  -198,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,     0,  -200
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    46,   269,    32,    33,    34,   128,
     164,   192,   161,   236,   258,   198,    35,   227,   186,    68,
      69,    36,    37,   156,   157,    38,   119,   146,   147,   271,
     224,   184,   252,   273,   274,   284,   264,    39,   140,   141,
      71,    80,    81,   116,    72,    73,    74,   148,   151,   178,
     215,   179,   180,   181,   182,   216,   217,   218,    75,    76,
      40,    41,    42,    98,    77,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,   142,    90,   174,    70,    87,   104,   155,    91,    49,
      92,   107,    43,   233,    96,    44,    93,    94,   219,     4,
     100,   247,   248,   249,   242,   250,   234,    97,   243,   282,
     235,    51,    88,    89,    61,   133,   134,   101,    62,    63,
     187,    65,    52,    99,   110,   111,   105,   102,   103,  -155,
     114,    82,    83,    84,    85,    86,  -156,   239,   220,   234,
    -157,  -158,   106,   235,   272,  -159,   275,    49,   108,   109,
     110,   111,   279,   225,   283,   112,    64,    45,   163,   117,
     113,   143,   275,   118,   165,   166,    47,   120,    48,    51,
     122,   123,   261,   108,   109,   110,   111,   240,   121,   241,
      52,   130,   124,   127,   136,   137,   138,   139,   135,    82,
      83,    84,    85,    86,   144,   145,    79,   125,   149,   199,
      49,   153,   126,   129,   158,   131,    49,   162,   132,   150,
     115,   168,   154,   160,    64,   169,     4,   108,   109,   110,
     111,   167,    51,   170,   172,   171,   177,   173,    51,   117,
     183,   185,   188,    52,   193,   194,   195,   196,   197,    52,
     189,   190,    82,    83,    84,    85,    86,   200,    82,    83,
      84,    85,    86,   203,   191,   -80,   201,   202,   221,   108,
     109,   110,   111,   115,   204,   229,   223,    64,   158,    49,
     222,   162,   226,    64,   230,   232,   237,   246,   251,   253,
      50,   219,   238,   117,   259,   262,   263,   257,   265,   267,
     278,    51,   244,   245,   268,   177,   177,   270,   -81,   177,
     281,    95,    52,   256,   285,   231,   276,   266,   228,   254,
     255,    53,    54,    55,    56,    57,    49,   260,   117,   286,
       4,    58,    59,    60,   280,   159,   175,    50,   176,     0,
      61,   152,     0,     0,    62,    63,    64,    65,    51,     0,
      66,    67,     0,     0,     0,     0,   177,     0,   277,    52,
       0,     0,    49,     0,     0,     0,     0,     0,    53,    54,
      55,    56,    57,    50,     0,     0,     0,     0,    58,    59,
      60,     0,     0,     0,    51,     0,     0,    61,     0,     0,
       0,    62,    63,    64,    65,    52,     0,    66,    67,    49,
       0,     0,     0,     0,    53,    54,    55,    56,    57,     0,
       0,     0,     0,     0,    58,    59,    60,     0,     0,     0,
       0,    51,     0,    61,     0,     0,     0,    62,    63,    64,
      65,     0,    52,    66,    67,     0,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,     1,     2,     0,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
       0,     0,   115,    11,    12,    13,    64,     0,     0,     0,
       0,    14,    15,   205,   206,   207,   208,   209,   210,    16,
       0,    17,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   211,   212,     0,   213,     0,   214,
       0,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111
};

static const yytype_int16 yycheck[] =
{
       4,   113,    32,   149,     3,     5,    50,   123,    34,     7,
      10,    20,     6,    18,     0,     9,    16,    40,    36,    11,
      74,   219,   220,   222,    64,   223,    64,     3,    68,    12,
      68,    29,     7,     8,    68,    19,    20,     8,    72,    73,
     156,    75,    40,    43,    79,    80,    50,    47,    48,    18,
      31,    49,    50,    51,    52,    53,    18,   203,    76,    64,
      18,    18,    66,    68,   263,    18,   265,     7,    77,    78,
      79,    80,   270,   185,    57,    18,    74,    71,    70,    79,
      18,    79,   281,    34,   128,   129,     6,    20,     8,    29,
      90,    91,   238,    77,    78,    79,    80,    65,    34,    67,
      40,   101,    37,    18,   108,   109,   110,   111,   107,    49,
      50,    51,    52,    53,   114,   115,   120,    43,   118,   163,
       7,   121,    41,    70,   124,    38,     7,   127,    19,    35,
      70,   131,    33,    75,    74,   134,    11,    77,    78,    79,
      80,    38,    29,    19,    62,    19,   150,    20,    29,   149,
      54,    18,    20,    40,    24,    25,    26,    27,    28,    40,
      43,    32,    49,    50,    51,    52,    53,   167,    49,    50,
      51,    52,    53,   173,    20,    62,    19,    63,   182,    77,
      78,    79,    80,    70,    66,   189,    58,    74,   188,     7,
      56,   191,    20,    74,     6,    19,    18,    75,    55,    19,
      18,    36,   202,   203,    68,    68,    20,    72,    56,    19,
      19,    29,   216,   217,    20,   219,   220,    38,    62,   223,
      20,    18,    40,   231,   277,   191,   267,   254,   188,   226,
     230,    49,    50,    51,    52,    53,     7,   237,   238,   281,
      11,    59,    60,    61,   272,   125,    64,    18,    66,    -1,
      68,   120,    -1,    -1,    72,    73,    74,    75,    29,    -1,
      78,    79,    -1,    -1,    -1,    -1,   270,    -1,   268,    40,
      -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    18,    -1,    -1,    -1,    -1,    59,    60,
      61,    -1,    -1,    -1,    29,    -1,    -1,    68,    -1,    -1,
      -1,    72,    73,    74,    75,    40,    -1,    78,    79,     7,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    -1,    -1,    -1,
      -1,    29,    -1,    68,    -1,    -1,    -1,    72,    73,    74,
      75,    -1,    40,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,     4,     5,    -1,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    70,    21,    22,    23,    74,    -1,    -1,    -1,
      -1,    29,    30,    43,    44,    45,    46,    47,    48,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    67,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      80
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,    10,    11,    12,    13,    14,    15,    16,
      17,    21,    22,    23,    29,    30,    37,    39,    42,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    98,    99,   100,   108,   113,   114,   117,   129,
     152,   153,   154,     6,     9,    71,    96,     6,     8,     7,
      18,    29,    40,    49,    50,    51,    52,    53,    59,    60,
      61,    68,    72,    73,    74,    75,    78,    79,   111,   112,
     130,   132,   136,   137,   138,   150,   151,   156,   157,   132,
     133,   134,    49,    50,    51,    52,    53,   156,     7,     8,
      32,    34,   156,   156,    40,    84,     0,     3,   155,   156,
      74,     8,   156,   156,   117,   132,   132,    20,    77,    78,
      79,    80,    18,    18,    31,    70,   135,   156,    34,   118,
      20,    34,   156,   156,    37,    43,    41,    18,   101,    70,
     156,    38,    19,    19,    20,   130,   132,   132,   132,   132,
     130,   131,   131,    79,   156,   156,   119,   120,   139,   156,
      35,   140,   133,   156,    33,   140,   115,   116,   156,   111,
      75,   104,   156,    70,   102,   117,   117,    38,   156,   130,
      19,    19,    62,    20,   135,    64,    66,   132,   141,   143,
     144,   145,   146,    54,   123,    18,   110,   140,    20,    43,
      32,    20,   103,    24,    25,    26,    27,    28,   107,   117,
     156,    19,    63,   156,    66,    43,    44,    45,    46,    47,
      48,    64,    65,    67,    69,   142,   147,   148,   149,    36,
      76,   132,    56,    58,   122,   131,    20,   109,   115,   132,
       6,   104,    19,    18,    64,    68,   105,    18,   156,   135,
      65,    67,    64,    68,   132,   132,    75,   141,   141,   138,
     141,    55,   124,    19,   110,   156,   103,    72,   106,    68,
     156,   135,    68,    20,   128,    56,   109,    19,    20,    97,
      38,   121,   138,   125,   126,   138,   105,   156,    19,   141,
     128,    20,    12,    57,   127,    97,   125
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    96,    97,    97,
      98,    99,   100,   101,   101,   102,   102,   102,   103,   103,
     104,   104,   105,   105,   105,   106,   107,   107,   107,   107,
     107,   108,   109,   109,   110,   111,   111,   111,   111,   112,
     113,   114,   115,   115,   116,   117,   118,   118,   118,   119,
     120,   120,   121,   122,   122,   123,   123,   124,   124,   125,
     125,   126,   127,   127,   127,   128,   128,   129,   130,   130,
     131,   131,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   133,   133,   134,   135,   135,
     135,   136,   137,   138,   138,   138,   139,   139,   140,   140,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     143,   144,   145,   146,   146,   147,   147,   147,   147,   147,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   150,
     151,   151,   151,   152,   153,   154,   155,   155,   156,   156,
     157,   157,   157,   157,   157,   157,   157,   157
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     2,     4,    10,     0,     1,     0,     3,
       5,     5,     5,     0,     4,     0,     2,     1,     0,     3,
       6,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     6,     0,     3,     3,     1,     1,     1,     1,     1,
       4,     5,     1,     3,     3,     7,     0,     2,     2,     6,
       1,     6,     2,     0,     2,     0,     4,     0,     3,     1,
       3,     2,     0,     1,     1,     0,     3,     2,     1,     3,
       0,     1,     3,     3,     3,     3,     3,     2,     1,     1,
       1,     4,     4,     1,     1,     1,     3,     2,     0,     1,
       2,     3,     5,     1,     3,     3,     2,     4,     0,     2,
       0,     1,     1,     3,     1,     2,     3,     3,     2,     3,
       3,     3,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     2,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 260 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1918 "yacc_sql.cpp"
    break;

  case 25: /* exit_stmt: EXIT  */
#line 292 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1927 "yacc_sql.cpp"
    break;

  case 26: /* help_stmt: HELP  */
#line 298 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1935 "yacc_sql.cpp"
    break;

  case 27: /* sync_stmt: SYNC  */
#line 303 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1943 "yacc_sql.cpp"
    break;

  case 28: /* begin_stmt: TRX_BEGIN  */
#line 309 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1951 "yacc_sql.cpp"
    break;

  case 29: /* commit_stmt: TRX_COMMIT  */
#line 315 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1959 "yacc_sql.cpp"
    break;

  case 30: /* rollback_stmt: TRX_ROLLBACK  */
#line 321 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1967 "yacc_sql.cpp"
    break;

  case 31: /* drop_table_stmt: DROP TABLE id  */
#line 327 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      auto *drop_table = new DropTableSqlNode;
      (yyval.sql_node)->node.drop_table = drop_table;
      drop_table->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1979 "yacc_sql.cpp"
    break;

  case 32: /* show_tables_stmt: SHOW TABLES  */
#line 336 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1987 "yacc_sql.cpp"
    break;

  case 33: /* desc_table_stmt: DESC id  */
#line 342 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      auto *desc_table = new DescTableSqlNode;
      (yyval.sql_node)->node.desc_table = desc_table;
      desc_table->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1999 "yacc_sql.cpp"
    break;

  case 34: /* show_index_stmt: SHOW INDEX FROM id  */
#line 352 "yacc_sql.y"
                       {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      auto *show_index = new ShowIndexSqlNode;
      (yyval.sql_node)->node.show_index = show_index;
      show_index->table_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2011 "yacc_sql.cpp"
    break;

  case 35: /* create_index_stmt: CREATE unique INDEX id ON id LBRACE id ids RBRACE  */
#line 363 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode *create_index = new CreateIndexSqlNode;
      (yyval.sql_node)->node.create_index = create_index;
      create_index->unique = (yyvsp[-8].bools);
      create_index->index_name = (yyvsp[-6].string);
      create_index->relation_name = (yyvsp[-4].string);
      (yyvsp[-1].id_list)->push_back((yyvsp[-2].string));
      create_index->attribute_names.swap(*(yyvsp[-1].id_list));
      delete (yyvsp[-1].id_list);
      std::reverse(create_index->attribute_names.begin(), create_index->attribute_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2031 "yacc_sql.cpp"
    break;

  case 36: /* unique: %empty  */
#line 381 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 2039 "yacc_sql.cpp"
    break;

  case 37: /* unique: UNIQUE  */
#line 384 "yacc_sql.y"
             {
      (yyval.bools) = true;
    }
#line 2047 "yacc_sql.cpp"
    break;

  case 38: /* ids: %empty  */
#line 389 "yacc_sql.y"
   {
      (yyval.id_list) = new std::vector<std::string>();
   }
#line 2055 "yacc_sql.cpp"
    break;

  case 39: /* ids: COMMA id ids  */
#line 392 "yacc_sql.y"
                  {
      (yyvsp[0].id_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
      (yyval.id_list) = (yyvsp[0].id_list);
   }
#line 2065 "yacc_sql.cpp"
    break;

  case 40: /* drop_index_stmt: DROP INDEX id ON id  */
#line 400 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      auto *drop_index = new DropIndexSqlNode;
      (yyval.sql_node)->node.drop_index = drop_index;
      drop_index->index_name = (yyvsp[-2].string);
      drop_index->relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2079 "yacc_sql.cpp"
    break;

  case 41: /* create_table_stmt: CREATE TABLE id attr_list as_select  */
#line 413 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode *create_table = new CreateTableSqlNode;
      (yyval.sql_node)->node.create_table = create_table;
      create_table->relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      if((yyvsp[-1].attr_infos) != nullptr) {
        create_table->attr_infos.swap(*(yyvsp[-1].attr_infos));
        delete (yyvsp[-1].attr_infos);
      }
      create_table->select = (yyvsp[0].sql_node);
    }
#line 2096 "yacc_sql.cpp"
    break;

  case 42: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 429 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_VIEW);
      CreateViewSqlNode *create_view = new CreateViewSqlNode;
      (yyval.sql_node)->node.create_view = create_view;
      create_view->view_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      create_view->select = (yyvsp[0].sql_node);
      create_view->select_sql = (yyvsp[0].sql_node)->node.selection->sql;
    }
#line 2110 "yacc_sql.cpp"
    break;

  case 43: /* attr_list: %empty  */
#line 440 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2118 "yacc_sql.cpp"
    break;

  case 44: /* attr_list: LBRACE attr_def attr_def_list RBRACE  */
#line 443 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].attr_infos) == nullptr) {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      } else {
        (yyval.attr_infos) = (yyvsp[-1].attr_infos);
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-2].attr_info));
      std::reverse((yyval.attr_infos)->begin(), (yyval.attr_infos)->end());
    }
#line 2132 "yacc_sql.cpp"
    break;

  case 45: /* as_select: %empty  */
#line 454 "yacc_sql.y"
    {
      (yyval.sql_node) = nullptr;
    }
#line 2140 "yacc_sql.cpp"
    break;

  case 46: /* as_select: AS select_stmt  */
#line 457 "yacc_sql.y"
                     {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2148 "yacc_sql.cpp"
    break;

  case 47: /* as_select: select_stmt  */
#line 460 "yacc_sql.y"
                  {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2156 "yacc_sql.cpp"
    break;

  case 48: /* attr_def_list: %empty  */
#line 466 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2164 "yacc_sql.cpp"
    break;

  case 49: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 470 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2178 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: id type LBRACE number RBRACE null_def  */
#line 483 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-5].string));
    }
#line 2191 "yacc_sql.cpp"
    break;

  case 51: /* attr_def: id type null_def  */
#line 492 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-2].string));
    }
#line 2204 "yacc_sql.cpp"
    break;

  case 52: /* null_def: %empty  */
#line 503 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 2212 "yacc_sql.cpp"
    break;

  case 53: /* null_def: NOT NULL_V  */
#line 506 "yacc_sql.y"
                 {
      (yyval.bools) = false;
    }
#line 2220 "yacc_sql.cpp"
    break;

  case 54: /* null_def: NULL_V  */
#line 509 "yacc_sql.y"
             {
      (yyval.bools) = true;
    }
#line 2228 "yacc_sql.cpp"
    break;

  case 55: /* number: NUMBER  */
#line 514 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2234 "yacc_sql.cpp"
    break;

  case 56: /* type: INT_T  */
#line 518 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2240 "yacc_sql.cpp"
    break;

  case 57: /* type: STRING_T  */
#line 519 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2246 "yacc_sql.cpp"
    break;

  case 58: /* type: FLOAT_T  */
#line 520 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2252 "yacc_sql.cpp"
    break;

  case 59: /* type: DATE_T  */
#line 521 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2258 "yacc_sql.cpp"
    break;

  case 60: /* type: TEXT_T  */
#line 522 "yacc_sql.y"
               { (yyval.number)=TEXTS; }
#line 2264 "yacc_sql.cpp"
    break;

  case 61: /* insert_stmt: INSERT INTO id VALUES record record_list  */
#line 527 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      auto *insertion = new InsertSqlNode;
      (yyval.sql_node)->node.insertion = insertion;
      insertion->relation_name = (yyvsp[-3].string);
      if ((yyvsp[0].record_list) != nullptr) {
        insertion->values.swap(*(yyvsp[0].record_list));
        delete (yyvsp[0].record_list);
      }
      insertion->values.emplace_back(*(yyvsp[-1].expression_list));
      delete (yyvsp[-1].expression_list);
      std::reverse(insertion->values.begin(), insertion->values.end());
      free((yyvsp[-3].string));
    }
#line 2283 "yacc_sql.cpp"
    break;

  case 62: /* record_list: %empty  */
#line 544 "yacc_sql.y"
    {
      (yyval.record_list) = nullptr;
    }
#line 2291 "yacc_sql.cpp"
    break;

  case 63: /* record_list: COMMA record record_list  */
#line 547 "yacc_sql.y"
                               {
      if ((yyvsp[0].record_list) != nullptr) {
        (yyval.record_list) = (yyvsp[0].record_list);
      } else {
        (yyval.record_list) = new std::vector<std::vector<ExprSqlNode *>>;
      }
      (yyval.record_list)->emplace_back(*(yyvsp[-1].expression_list));
      delete (yyvsp[-1].expression_list);
    }
#line 2305 "yacc_sql.cpp"
    break;

  case 64: /* record: LBRACE expression_list_empty RBRACE  */
#line 559 "yacc_sql.y"
    {
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[-1].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      }
      reverse((yyval.expression_list)->begin(), (yyval.expression_list)->end());
    }
#line 2318 "yacc_sql.cpp"
    break;

  case 65: /* value: NUMBER  */
#line 569 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2327 "yacc_sql.cpp"
    break;

  case 66: /* value: FLOAT  */
#line 573 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2336 "yacc_sql.cpp"
    break;

  case 67: /* value: SSS  */
#line 577 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2346 "yacc_sql.cpp"
    break;

  case 68: /* value: NULL_V  */
#line 582 "yacc_sql.y"
             {
      (yyval.value) = new Value;
      (yyval.value)->set_null();
    }
#line 2355 "yacc_sql.cpp"
    break;

  case 69: /* value_expr: value  */
#line 589 "yacc_sql.y"
          {
      (yyval.value_expr) = new ValueExprSqlNode;
      (yyval.value_expr)->value = *(yyvsp[0].value);
      delete (yyvsp[0].value);
    }
#line 2365 "yacc_sql.cpp"
    break;

  case 70: /* delete_stmt: DELETE FROM id where  */
#line 597 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      auto *deletion = new DeleteSqlNode;
      (yyval.sql_node)->node.deletion = deletion;
      deletion->relation_name = (yyvsp[-1].string);
      deletion->conditions = (yyvsp[0].conjunction);
      free((yyvsp[-1].string));
    }
#line 2378 "yacc_sql.cpp"
    break;

  case 71: /* update_stmt: UPDATE id SET update_set_list where  */
#line 609 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      auto *update = new UpdateSqlNode;
      (yyval.sql_node)->node.update = update;
      update->relation_name = (yyvsp[-3].string);
      update->sets.swap(*(yyvsp[-1].update_set_list));
      delete (yyvsp[-1].update_set_list);
      update->conditions = (yyvsp[0].conjunction);
      free((yyvsp[-3].string));
    }
#line 2393 "yacc_sql.cpp"
    break;

  case 72: /* update_set_list: update_set  */
#line 623 "yacc_sql.y"
    {
      (yyval.update_set_list) = new std::vector<UpdateSetSqlNode *>(1, (yyvsp[0].update_set));
    }
#line 2401 "yacc_sql.cpp"
    break;

  case 73: /* update_set_list: update_set COMMA update_set_list  */
#line 626 "yacc_sql.y"
                                       {
      (yyval.update_set_list) = (yyvsp[0].update_set_list);
      (yyval.update_set_list)->push_back((yyvsp[-2].update_set));
    }
#line 2410 "yacc_sql.cpp"
    break;

  case 74: /* update_set: id EQ expression  */
#line 632 "yacc_sql.y"
                     {
      (yyval.update_set) = new UpdateSetSqlNode;
      (yyval.update_set)->field_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.update_set)->expr = (yyvsp[0].expression);
    }
#line 2421 "yacc_sql.cpp"
    break;

  case 75: /* select_stmt: SELECT select_attr_list from where groupby having orderby  */
#line 641 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      auto* selection = new SelectSqlNode;
      (yyval.sql_node)->node.selection = selection;
      if ((yyvsp[-5].select_attr_list) != nullptr) {
        std::reverse((yyvsp[-5].select_attr_list)->begin(), (yyvsp[-5].select_attr_list)->end());
        selection->attributes.swap(*(yyvsp[-5].select_attr_list));
        delete (yyvsp[-5].select_attr_list);
      }
      if ((yyvsp[-4].join) != nullptr) {
        selection->tables=(yyvsp[-4].join);
      }
      if ((yyvsp[-2].rel_attr_list) != nullptr) {
        selection->groupbys.swap(*(yyvsp[-2].rel_attr_list));
        delete (yyvsp[-2].rel_attr_list);
      }
      if ((yyvsp[0].order_unit_list) != nullptr) {
        selection->orderbys.swap(*(yyvsp[0].order_unit_list));
        delete (yyvsp[0].order_unit_list);
      }

      selection->conditions = (yyvsp[-3].conjunction);
      selection->having_conditions=(yyvsp[-1].conjunction);
      selection->sql = token_name(sql_string, &(yyloc));
    }
#line 2451 "yacc_sql.cpp"
    break;

  case 76: /* from: %empty  */
#line 669 "yacc_sql.y"
    {
      (yyval.join) = nullptr;
    }
#line 2459 "yacc_sql.cpp"
    break;

  case 77: /* from: FROM rel_list  */
#line 672 "yacc_sql.y"
                    {
      (yyval.join) = (yyvsp[0].join);
    }
#line 2467 "yacc_sql.cpp"
    break;

  case 78: /* from: FROM joined_tables  */
#line 675 "yacc_sql.y"
                         {
      (yyval.join) = (yyvsp[0].join);
    }
#line 2475 "yacc_sql.cpp"
    break;

  case 79: /* joined_tables: joined_tables_inner INNER JOIN id as_info joined_on  */
#line 681 "yacc_sql.y"
                                                        {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation=(yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.join)->alias = (yyvsp[-1].string);
      if(*(yyvsp[-1].string)) free((yyvsp[-1].string));
      (yyval.join)->sub_join=(yyvsp[-5].join);
      (yyval.join)->join_conditions=(yyvsp[0].conjunction);  
    }
#line 2489 "yacc_sql.cpp"
    break;

  case 80: /* joined_tables_inner: id  */
#line 692 "yacc_sql.y"
       {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2499 "yacc_sql.cpp"
    break;

  case 81: /* joined_tables_inner: joined_tables_inner INNER JOIN id as_info joined_on  */
#line 697 "yacc_sql.y"
                                                          {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation=(yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.join)->sub_join=(yyvsp[-5].join);
      (yyval.join)->alias=(yyvsp[-1].string);
      if(*(yyvsp[-1].string)) free((yyvsp[-1].string));
      (yyval.join)->join_conditions=(yyvsp[0].conjunction);  
    }
#line 2513 "yacc_sql.cpp"
    break;

  case 82: /* joined_on: ON conjunction  */
#line 708 "yacc_sql.y"
                   {
      (yyval.conjunction) = (yyvsp[0].conjunction);
    }
#line 2521 "yacc_sql.cpp"
    break;

  case 83: /* having: %empty  */
#line 713 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2529 "yacc_sql.cpp"
    break;

  case 84: /* having: HAVING conjunction  */
#line 716 "yacc_sql.y"
                         {
      (yyval.conjunction) = (yyvsp[0].conjunction);
    }
#line 2537 "yacc_sql.cpp"
    break;

  case 85: /* groupby: %empty  */
#line 722 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2545 "yacc_sql.cpp"
    break;

  case 86: /* groupby: GROUP BY rel_attr rel_attr_list  */
#line 726 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      if ((yyval.rel_attr_list) == nullptr) {
        (yyval.rel_attr_list) = new std::vector<FieldExprSqlNode *>();
      }
      (yyval.rel_attr_list)->push_back((yyvsp[-1].rel_attr));
      std::reverse((yyval.rel_attr_list)->begin(), (yyval.rel_attr_list)->end());
    }
#line 2558 "yacc_sql.cpp"
    break;

  case 87: /* orderby: %empty  */
#line 736 "yacc_sql.y"
    {
      (yyval.order_unit_list) = nullptr;
    }
#line 2566 "yacc_sql.cpp"
    break;

  case 88: /* orderby: ORDER BY order_unit_list  */
#line 739 "yacc_sql.y"
                               {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      std::reverse((yyval.order_unit_list)->begin(), (yyval.order_unit_list)->end());
    }
#line 2575 "yacc_sql.cpp"
    break;

  case 89: /* order_unit_list: order_unit  */
#line 746 "yacc_sql.y"
    {
      (yyval.order_unit_list) = new std::vector<OrderBySqlNode *>();
      (yyval.order_unit_list)->push_back((yyvsp[0].order_unit));
    }
#line 2584 "yacc_sql.cpp"
    break;

  case 90: /* order_unit_list: order_unit COMMA order_unit_list  */
#line 751 "yacc_sql.y"
    {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      (yyval.order_unit_list)->push_back((yyvsp[-2].order_unit));
    }
#line 2593 "yacc_sql.cpp"
    break;

  case 91: /* order_unit: rel_attr order  */
#line 757 "yacc_sql.y"
                   {
      (yyval.order_unit) = new OrderBySqlNode;
      (yyval.order_unit)->field = (yyvsp[-1].rel_attr);
      (yyval.order_unit)->order = (yyvsp[0].order);
    }
#line 2603 "yacc_sql.cpp"
    break;

  case 92: /* order: %empty  */
#line 764 "yacc_sql.y"
    {
      (yyval.order) = Order::ASC;
    }
#line 2611 "yacc_sql.cpp"
    break;

  case 93: /* order: ASC  */
#line 767 "yacc_sql.y"
          {
      (yyval.order) = Order::ASC;
    }
#line 2619 "yacc_sql.cpp"
    break;

  case 94: /* order: DESC  */
#line 770 "yacc_sql.y"
           {
      (yyval.order) = Order::DESC;
    }
#line 2627 "yacc_sql.cpp"
    break;

  case 95: /* rel_attr_list: %empty  */
#line 775 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2635 "yacc_sql.cpp"
    break;

  case 96: /* rel_attr_list: COMMA rel_attr rel_attr_list  */
#line 779 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      if ((yyval.rel_attr_list) == nullptr) {
        (yyval.rel_attr_list) = new std::vector<FieldExprSqlNode *>();
      }
      (yyval.rel_attr_list)->push_back((yyvsp[-1].rel_attr));
    }
#line 2647 "yacc_sql.cpp"
    break;

  case 97: /* calc_stmt: CALC expression_list  */
#line 789 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      auto *tmp = new CalcSqlNode;
      (yyval.sql_node)->node.calc = tmp;
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      tmp->expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2660 "yacc_sql.cpp"
    break;

  case 98: /* expression_list: expression  */
#line 801 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2669 "yacc_sql.cpp"
    break;

  case 99: /* expression_list: expression COMMA expression_list  */
#line 806 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2682 "yacc_sql.cpp"
    break;

  case 100: /* expression_list_empty: %empty  */
#line 817 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2690 "yacc_sql.cpp"
    break;

  case 101: /* expression_list_empty: expression_list  */
#line 820 "yacc_sql.y"
                      {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2698 "yacc_sql.cpp"
    break;

  case 102: /* expression: expression '+' expression  */
#line 825 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2706 "yacc_sql.cpp"
    break;

  case 103: /* expression: expression '-' expression  */
#line 828 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2714 "yacc_sql.cpp"
    break;

  case 104: /* expression: expression '*' expression  */
#line 831 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2722 "yacc_sql.cpp"
    break;

  case 105: /* expression: expression '/' expression  */
#line 834 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2730 "yacc_sql.cpp"
    break;

  case 106: /* expression: LBRACE expression RBRACE  */
#line 837 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2739 "yacc_sql.cpp"
    break;

  case 107: /* expression: '-' expression  */
#line 841 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2747 "yacc_sql.cpp"
    break;

  case 108: /* expression: '*'  */
#line 844 "yacc_sql.y"
          {
      (yyval.expression) = new ExprSqlNode(new StarExprSqlNode);
    }
#line 2755 "yacc_sql.cpp"
    break;

  case 109: /* expression: rel_attr  */
#line 847 "yacc_sql.y"
               {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].rel_attr));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2764 "yacc_sql.cpp"
    break;

  case 110: /* expression: value_expr  */
#line 851 "yacc_sql.y"
                 {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].value_expr));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2773 "yacc_sql.cpp"
    break;

  case 111: /* expression: aggr_op LBRACE expression_list_empty RBRACE  */
#line 855 "yacc_sql.y"
                                                  {
      std::string name = token_name(sql_string, &(yyloc));
      if ((yyvsp[-1].expression_list)) {
        (yyval.expression) = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode((yyvsp[-3].aggr), *(yyvsp[-1].expression_list))));
      } else {
        (yyval.expression) = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode((yyvsp[-3].aggr))));
      }
      (yyval.expression)->set_name(name);
    }
#line 2787 "yacc_sql.cpp"
    break;

  case 112: /* expression: func_op LBRACE expression_list_empty RBRACE  */
#line 864 "yacc_sql.y"
                                                  {
      std::string name = token_name(sql_string, &(yyloc));
      reverse((yyvsp[-1].expression_list)->begin(), (yyvsp[-1].expression_list)->end());
      (yyval.expression) = new ExprSqlNode(new FunctionExprSqlNode((yyvsp[-3].func), (yyvsp[-1].expression_list)));
      delete (yyvsp[-1].expression_list);
      (yyval.expression)->set_name(name);
    }
#line 2799 "yacc_sql.cpp"
    break;

  case 113: /* expression: list_expr  */
#line 871 "yacc_sql.y"
                {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].list));
      std::string name = token_name(sql_string, &(yyloc));
      (yyval.expression)->set_name(name);
    }
#line 2809 "yacc_sql.cpp"
    break;

  case 114: /* expression: set_expr  */
#line 876 "yacc_sql.y"
               {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].set));
      std::string name = token_name(sql_string, &(yyloc));
      (yyval.expression)->set_name(name);
    }
#line 2819 "yacc_sql.cpp"
    break;

  case 115: /* select_attr_list: select_attr  */
#line 884 "yacc_sql.y"
                {
      (yyval.select_attr_list) = new std::vector<SelectAttribute *>(1, (yyvsp[0].select_attr));
    }
#line 2827 "yacc_sql.cpp"
    break;

  case 116: /* select_attr_list: select_attr COMMA select_attr_list  */
#line 887 "yacc_sql.y"
                                         {
      (yyvsp[0].select_attr_list)->push_back((yyvsp[-2].select_attr));
      (yyval.select_attr_list) = (yyvsp[0].select_attr_list);
    }
#line 2836 "yacc_sql.cpp"
    break;

  case 117: /* select_attr: expression as_info  */
#line 894 "yacc_sql.y"
                       {
      (yyval.select_attr) = new SelectAttribute;
      (yyval.select_attr)->expr = (yyvsp[-1].expression);
      (yyval.select_attr)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
    }
#line 2847 "yacc_sql.cpp"
    break;

  case 118: /* as_info: %empty  */
#line 902 "yacc_sql.y"
    {
      (yyval.string) = "";
    }
#line 2855 "yacc_sql.cpp"
    break;

  case 119: /* as_info: id  */
#line 905 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2863 "yacc_sql.cpp"
    break;

  case 120: /* as_info: AS id  */
#line 908 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2871 "yacc_sql.cpp"
    break;

  case 121: /* list_expr: LBRACE select_stmt RBRACE  */
#line 913 "yacc_sql.y"
                              {
      (yyval.list) = new ListExprSqlNode((yyvsp[-1].sql_node)->node.selection);
      (yyvsp[-1].sql_node)->node.selection = nullptr;
      delete (yyvsp[-1].sql_node);
    }
#line 2881 "yacc_sql.cpp"
    break;

  case 122: /* set_expr: LBRACE expression COMMA expression_list RBRACE  */
#line 921 "yacc_sql.y"
                                                   {
      (yyvsp[-1].expression_list)->push_back((yyvsp[-3].expression));
      (yyval.set) = new SetExprSqlNode();
      (yyval.set)->expressions.swap(*(yyvsp[-1].expression_list));
      delete (yyvsp[-1].expression_list);
    }
#line 2892 "yacc_sql.cpp"
    break;

  case 123: /* rel_attr: id  */
#line 930 "yacc_sql.y"
       {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->field_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2902 "yacc_sql.cpp"
    break;

  case 124: /* rel_attr: id DOT id  */
#line 935 "yacc_sql.y"
                {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->table_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->field_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2914 "yacc_sql.cpp"
    break;

  case 125: /* rel_attr: id DOT '*'  */
#line 942 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->table_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->field_name = "*";
      free((yyvsp[-2].string));
    }
#line 2925 "yacc_sql.cpp"
    break;

  case 126: /* rel_list: id as_info  */
#line 952 "yacc_sql.y"
               {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[-1].string);
      (yyval.join)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
      free((yyvsp[-1].string));
    }
#line 2937 "yacc_sql.cpp"
    break;

  case 127: /* rel_list: rel_list COMMA id as_info  */
#line 959 "yacc_sql.y"
                                {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      (yyval.join)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
      (yyval.join)->sub_join = (yyvsp[-3].join);
    }
#line 2950 "yacc_sql.cpp"
    break;

  case 128: /* where: %empty  */
#line 971 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2958 "yacc_sql.cpp"
    break;

  case 129: /* where: WHERE conjunction  */
#line 974 "yacc_sql.y"
                        {
      (yyval.conjunction) = (yyvsp[0].conjunction);  
    }
#line 2966 "yacc_sql.cpp"
    break;

  case 130: /* conjunction: %empty  */
#line 980 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2974 "yacc_sql.cpp"
    break;

  case 131: /* conjunction: contain  */
#line 983 "yacc_sql.y"
              {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].contain), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2982 "yacc_sql.cpp"
    break;

  case 132: /* conjunction: condition  */
#line 986 "yacc_sql.y"
                {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].condition), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2990 "yacc_sql.cpp"
    break;

  case 133: /* conjunction: expression like_op SSS  */
#line 989 "yacc_sql.y"
                             {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new LikeExprSqlNode((yyvsp[-1].bools), (yyvsp[-2].expression), (yyvsp[0].string)), static_cast<ExprSqlNode *>(nullptr));
      free((yyvsp[0].string));
    }
#line 2999 "yacc_sql.cpp"
    break;

  case 134: /* conjunction: exists  */
#line 993 "yacc_sql.y"
             {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].exists), static_cast<ExprSqlNode *>(nullptr));
    }
#line 3007 "yacc_sql.cpp"
    break;

  case 135: /* conjunction: expression null_check  */
#line 996 "yacc_sql.y"
                            {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new NullCheckExprSqlNode((yyvsp[0].bools), (yyvsp[-1].expression)), static_cast<ExprSqlNode *>(nullptr));
    }
#line 3015 "yacc_sql.cpp"
    break;

  case 136: /* conjunction: conjunction AND conjunction  */
#line 999 "yacc_sql.y"
                                  {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::AND, (yyvsp[-2].conjunction), (yyvsp[0].conjunction));
    }
#line 3023 "yacc_sql.cpp"
    break;

  case 137: /* conjunction: conjunction OR conjunction  */
#line 1002 "yacc_sql.y"
                                 {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::OR, (yyvsp[-2].conjunction), (yyvsp[0].conjunction));
    }
#line 3031 "yacc_sql.cpp"
    break;

  case 138: /* null_check: IS NULL_V  */
#line 1008 "yacc_sql.y"
              {
      (yyval.bools) = true;
    }
#line 3039 "yacc_sql.cpp"
    break;

  case 139: /* null_check: IS NOT NULL_V  */
#line 1011 "yacc_sql.y"
                    {
      (yyval.bools) = false;
    }
#line 3047 "yacc_sql.cpp"
    break;

  case 140: /* condition: expression comp_op expression  */
#line 1016 "yacc_sql.y"
                                  {
      (yyval.condition) = new ComparisonExprSqlNode((yyvsp[-1].comp), (yyvsp[-2].expression), (yyvsp[0].expression)); 
    }
#line 3055 "yacc_sql.cpp"
    break;

  case 141: /* contain: expression contain_op expression  */
#line 1022 "yacc_sql.y"
                                     {
      (yyval.contain) = new ContainExprSqlNode((yyvsp[-1].contain_op), (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 3063 "yacc_sql.cpp"
    break;

  case 142: /* exists: exists_op expression  */
#line 1028 "yacc_sql.y"
                         {
      (yyval.exists) = new ExistsExprSqlNode((yyvsp[-1].bools), (yyvsp[0].expression));
    }
#line 3071 "yacc_sql.cpp"
    break;

  case 143: /* exists_op: EXISTS  */
#line 1033 "yacc_sql.y"
           {
      (yyval.bools) = true;
    }
#line 3079 "yacc_sql.cpp"
    break;

  case 144: /* exists_op: NOT EXISTS  */
#line 1036 "yacc_sql.y"
                 {
      (yyval.bools) = false;
    }
#line 3087 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: EQ  */
#line 1041 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3093 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: LT  */
#line 1042 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3099 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: GT  */
#line 1043 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3105 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: LE  */
#line 1044 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3111 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: GE  */
#line 1045 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3117 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: NE  */
#line 1046 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3123 "yacc_sql.cpp"
    break;

  case 151: /* contain_op: IN  */
#line 1050 "yacc_sql.y"
         { (yyval.contain_op) = ContainType::IN; }
#line 3129 "yacc_sql.cpp"
    break;

  case 152: /* contain_op: NOT IN  */
#line 1051 "yacc_sql.y"
             { (yyval.contain_op) = ContainType::NOT_IN; }
#line 3135 "yacc_sql.cpp"
    break;

  case 153: /* like_op: LIKE  */
#line 1054 "yacc_sql.y"
           { (yyval.bools) = true; }
#line 3141 "yacc_sql.cpp"
    break;

  case 154: /* like_op: NOT LIKE  */
#line 1055 "yacc_sql.y"
               { (yyval.bools) = false; }
#line 3147 "yacc_sql.cpp"
    break;

  case 155: /* aggr_op: MIN  */
#line 1058 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_MIN; }
#line 3153 "yacc_sql.cpp"
    break;

  case 156: /* aggr_op: MAX  */
#line 1059 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_MAX; }
#line 3159 "yacc_sql.cpp"
    break;

  case 157: /* aggr_op: AVG  */
#line 1060 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_AVG; }
#line 3165 "yacc_sql.cpp"
    break;

  case 158: /* aggr_op: SUM  */
#line 1061 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_SUM; }
#line 3171 "yacc_sql.cpp"
    break;

  case 159: /* aggr_op: COUNT  */
#line 1062 "yacc_sql.y"
            { (yyval.aggr) = AggregationType::AGGR_COUNT; }
#line 3177 "yacc_sql.cpp"
    break;

  case 160: /* func_op: LENGTH  */
#line 1065 "yacc_sql.y"
             { (yyval.func) = FunctionType::LENGTH; }
#line 3183 "yacc_sql.cpp"
    break;

  case 161: /* func_op: ROUND  */
#line 1066 "yacc_sql.y"
            { (yyval.func) = FunctionType::ROUND; }
#line 3189 "yacc_sql.cpp"
    break;

  case 162: /* func_op: DATE_FORMAT  */
#line 1067 "yacc_sql.y"
                  { (yyval.func) = FunctionType::DATE_FORMAT; }
#line 3195 "yacc_sql.cpp"
    break;

  case 163: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE id  */
#line 1071 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      auto *load_data = new LoadDataSqlNode;
      (yyval.sql_node)->node.load_data = load_data;
      load_data->relation_name = (yyvsp[0].string);
      load_data->file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3211 "yacc_sql.cpp"
    break;

  case 164: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1086 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->node.explain = new ExplainSqlNode;
      (yyval.sql_node)->node.explain->sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3221 "yacc_sql.cpp"
    break;

  case 165: /* set_variable_stmt: SET id EQ value  */
#line 1095 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      auto *set_variable = new SetVariableSqlNode;
      (yyval.sql_node)->node.set_variable = set_variable;
      set_variable->name  = (yyvsp[-2].string);
      set_variable->value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3235 "yacc_sql.cpp"
    break;

  case 168: /* id: non_reserve  */
#line 1111 "yacc_sql.y"
                {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3243 "yacc_sql.cpp"
    break;

  case 169: /* id: ID  */
#line 1114 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3251 "yacc_sql.cpp"
    break;

  case 170: /* non_reserve: TABLES  */
#line 1119 "yacc_sql.y"
           {
      (yyval.string) = strdup("tables");
    }
#line 3259 "yacc_sql.cpp"
    break;

  case 171: /* non_reserve: HELP  */
#line 1122 "yacc_sql.y"
           {
      (yyval.string) = strdup("help");
    }
#line 3267 "yacc_sql.cpp"
    break;

  case 172: /* non_reserve: DATA  */
#line 1125 "yacc_sql.y"
           {
      (yyval.string) = strdup("data");
    }
#line 3275 "yacc_sql.cpp"
    break;

  case 173: /* non_reserve: MIN  */
#line 1128 "yacc_sql.y"
          {
      (yyval.string) = strdup("min");
    }
#line 3283 "yacc_sql.cpp"
    break;

  case 174: /* non_reserve: MAX  */
#line 1131 "yacc_sql.y"
          {
      (yyval.string) = strdup("max");
    }
#line 3291 "yacc_sql.cpp"
    break;

  case 175: /* non_reserve: AVG  */
#line 1134 "yacc_sql.y"
          {
      (yyval.string) = strdup("avg");
    }
#line 3299 "yacc_sql.cpp"
    break;

  case 176: /* non_reserve: SUM  */
#line 1137 "yacc_sql.y"
          {
      (yyval.string) = strdup("sum");
    }
#line 3307 "yacc_sql.cpp"
    break;

  case 177: /* non_reserve: COUNT  */
#line 1140 "yacc_sql.y"
            {
      (yyval.string) = strdup("count");
    }
#line 3315 "yacc_sql.cpp"
    break;


#line 3319 "yacc_sql.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1144 "yacc_sql.y"

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
