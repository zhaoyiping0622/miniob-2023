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
  YYSYMBOL_NUMBER = 71,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 72,                     /* FLOAT  */
  YYSYMBOL_ID = 73,                        /* ID  */
  YYSYMBOL_SSS = 74,                       /* SSS  */
  YYSYMBOL_OR = 75,                        /* OR  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '/'  */
  YYSYMBOL_UMINUS = 80,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 81,                  /* $accept  */
  YYSYMBOL_commands = 82,                  /* commands  */
  YYSYMBOL_command_wrapper = 83,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 84,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 85,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 86,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 87,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 88,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 89,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 90,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 91,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 92,           /* desc_table_stmt  */
  YYSYMBOL_show_index_stmt = 93,           /* show_index_stmt  */
  YYSYMBOL_create_index_stmt = 94,         /* create_index_stmt  */
  YYSYMBOL_unique = 95,                    /* unique  */
  YYSYMBOL_ids = 96,                       /* ids  */
  YYSYMBOL_drop_index_stmt = 97,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 98,         /* create_table_stmt  */
  YYSYMBOL_attr_list = 99,                 /* attr_list  */
  YYSYMBOL_as_select = 100,                /* as_select  */
  YYSYMBOL_attr_def_list = 101,            /* attr_def_list  */
  YYSYMBOL_attr_def = 102,                 /* attr_def  */
  YYSYMBOL_null_def = 103,                 /* null_def  */
  YYSYMBOL_number = 104,                   /* number  */
  YYSYMBOL_type = 105,                     /* type  */
  YYSYMBOL_insert_stmt = 106,              /* insert_stmt  */
  YYSYMBOL_record_list = 107,              /* record_list  */
  YYSYMBOL_record = 108,                   /* record  */
  YYSYMBOL_value = 109,                    /* value  */
  YYSYMBOL_value_expr = 110,               /* value_expr  */
  YYSYMBOL_delete_stmt = 111,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 112,              /* update_stmt  */
  YYSYMBOL_update_set_list = 113,          /* update_set_list  */
  YYSYMBOL_update_set = 114,               /* update_set  */
  YYSYMBOL_select_stmt = 115,              /* select_stmt  */
  YYSYMBOL_from = 116,                     /* from  */
  YYSYMBOL_joined_tables = 117,            /* joined_tables  */
  YYSYMBOL_joined_tables_inner = 118,      /* joined_tables_inner  */
  YYSYMBOL_joined_on = 119,                /* joined_on  */
  YYSYMBOL_having = 120,                   /* having  */
  YYSYMBOL_groupby = 121,                  /* groupby  */
  YYSYMBOL_orderby = 122,                  /* orderby  */
  YYSYMBOL_order_unit_list = 123,          /* order_unit_list  */
  YYSYMBOL_order_unit = 124,               /* order_unit  */
  YYSYMBOL_order = 125,                    /* order  */
  YYSYMBOL_rel_attr_list = 126,            /* rel_attr_list  */
  YYSYMBOL_calc_stmt = 127,                /* calc_stmt  */
  YYSYMBOL_expression_list = 128,          /* expression_list  */
  YYSYMBOL_expression_list_empty = 129,    /* expression_list_empty  */
  YYSYMBOL_expression = 130,               /* expression  */
  YYSYMBOL_select_attr_list = 131,         /* select_attr_list  */
  YYSYMBOL_select_attr = 132,              /* select_attr  */
  YYSYMBOL_as_info = 133,                  /* as_info  */
  YYSYMBOL_list_expr = 134,                /* list_expr  */
  YYSYMBOL_set_expr = 135,                 /* set_expr  */
  YYSYMBOL_rel_attr = 136,                 /* rel_attr  */
  YYSYMBOL_rel_list = 137,                 /* rel_list  */
  YYSYMBOL_where = 138,                    /* where  */
  YYSYMBOL_conjunction = 139,              /* conjunction  */
  YYSYMBOL_null_check = 140,               /* null_check  */
  YYSYMBOL_condition = 141,                /* condition  */
  YYSYMBOL_contain = 142,                  /* contain  */
  YYSYMBOL_exists = 143,                   /* exists  */
  YYSYMBOL_exists_op = 144,                /* exists_op  */
  YYSYMBOL_comp_op = 145,                  /* comp_op  */
  YYSYMBOL_contain_op = 146,               /* contain_op  */
  YYSYMBOL_like_op = 147,                  /* like_op  */
  YYSYMBOL_aggr_op = 148,                  /* aggr_op  */
  YYSYMBOL_func_op = 149,                  /* func_op  */
  YYSYMBOL_load_data_stmt = 150,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 151,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 152,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 153,            /* opt_semicolon  */
  YYSYMBOL_id = 154,                       /* id  */
  YYSYMBOL_non_reserve = 155               /* non_reserve  */
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
#define YYFINAL  94
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   398

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  175
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  282

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


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
       2,     2,    78,    76,     2,    77,     2,    79,     2,     2,
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
      75,    80
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   257,   257,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   289,   295,   300,   306,   312,   318,
     324,   333,   339,   349,   359,   378,   381,   386,   389,   396,
     409,   425,   428,   439,   442,   445,   451,   454,   467,   476,
     488,   491,   494,   499,   503,   504,   505,   506,   507,   511,
     529,   532,   543,   554,   558,   562,   567,   574,   581,   593,
     607,   611,   617,   625,   653,   656,   659,   665,   676,   681,
     692,   697,   700,   706,   709,   720,   723,   729,   734,   741,
     748,   751,   754,   759,   762,   772,   784,   789,   801,   804,
     809,   812,   815,   818,   821,   825,   828,   831,   835,   839,
     848,   855,   860,   868,   871,   878,   886,   889,   892,   897,
     905,   914,   919,   926,   936,   943,   955,   958,   964,   967,
     970,   973,   977,   980,   983,   986,   992,   995,  1000,  1006,
    1012,  1017,  1020,  1025,  1026,  1027,  1028,  1029,  1030,  1034,
    1035,  1038,  1039,  1042,  1043,  1044,  1045,  1046,  1049,  1050,
    1051,  1054,  1069,  1078,  1090,  1091,  1095,  1098,  1103,  1106,
    1109,  1112,  1115,  1118,  1121,  1124
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
  "IS", "AS", "NUMBER", "FLOAT", "ID", "SSS", "OR", "'+'", "'-'", "'*'",
  "'/'", "UMINUS", "$accept", "commands", "command_wrapper", "exit_stmt",
  "help_stmt", "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "show_index_stmt", "create_index_stmt", "unique", "ids",
  "drop_index_stmt", "create_table_stmt", "attr_list", "as_select",
  "attr_def_list", "attr_def", "null_def", "number", "type", "insert_stmt",
  "record_list", "record", "value", "value_expr", "delete_stmt",
  "update_stmt", "update_set_list", "update_set", "select_stmt", "from",
  "joined_tables", "joined_tables_inner", "joined_on", "having", "groupby",
  "orderby", "order_unit_list", "order_unit", "order", "rel_attr_list",
  "calc_stmt", "expression_list", "expression_list_empty", "expression",
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

#define YYPACT_NINF (-203)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-158)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     331,    20,    73,   260,   260,    33,     4,  -203,     9,    59,
      33,  -203,  -203,  -203,  -203,  -203,    33,    -8,   331,    97,
      96,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,
    -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,
    -203,  -203,    33,  -203,    92,    33,    33,  -203,   224,  -203,
    -203,    89,    90,    91,    98,   101,  -203,  -203,  -203,  -203,
    -203,  -203,  -203,  -203,   260,  -203,  -203,  -203,  -203,    71,
    -203,  -203,  -203,   102,   104,    94,  -203,    88,    95,   103,
    -203,  -203,  -203,  -203,  -203,  -203,  -203,   109,    33,    33,
      93,    83,   105,  -203,  -203,  -203,  -203,   118,    33,  -203,
     106,   136,    56,  -203,   260,   260,   260,   260,   260,   260,
     260,    14,    33,  -203,  -203,    33,   121,   260,    33,   126,
     121,    33,   -41,    86,    33,    -2,   125,    33,  -203,  -203,
     260,  -203,   -54,   -54,  -203,  -203,  -203,   151,   152,  -203,
    -203,  -203,  -203,   107,   154,    -1,   177,   122,  -203,  -203,
     157,  -203,   121,   158,   129,  -203,   145,   161,    10,   171,
    -203,  -203,    33,  -203,   166,  -203,  -203,   124,    33,  -203,
     123,  -203,   319,   -28,  -203,  -203,  -203,   260,   132,   133,
     260,   172,  -203,    33,   260,   187,    33,   175,  -203,  -203,
    -203,  -203,  -203,   -11,  -203,   178,  -203,    33,   150,  -203,
    -203,  -203,  -203,  -203,  -203,  -203,    29,  -203,  -203,   -51,
    -203,   260,   260,   130,   177,   177,    75,    33,   177,   160,
     186,   157,  -203,  -203,    75,    33,   161,  -203,   138,   144,
    -203,  -203,    33,   150,  -203,  -203,  -203,   148,  -203,    75,
      75,  -203,  -203,   182,   193,   -28,   163,  -203,  -203,   172,
    -203,  -203,  -203,   202,  -203,   204,   184,  -203,    33,  -203,
      33,  -203,   -46,    33,   215,   177,   185,   193,  -203,   219,
       2,  -203,   204,  -203,   -28,  -203,    33,  -203,  -203,  -203,
    -203,  -203
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    35,     0,     0,     0,     0,     0,    26,     0,     0,
       0,    27,    28,    29,    25,    24,     0,     0,     0,     0,
     164,    23,    22,    15,    16,    17,    18,     9,    10,    12,
      11,    13,    14,     8,     5,     7,     6,     4,     3,    19,
      20,    21,     0,    36,     0,     0,     0,   168,     0,   169,
     170,   171,   172,   173,   174,   175,   158,   159,   160,    66,
      63,    64,   167,    65,     0,   106,    67,   108,    95,    96,
     111,   112,   107,     0,     0,   121,   166,   116,    74,   113,
     171,   172,   173,   174,   175,    32,    31,     0,     0,     0,
       0,     0,     0,   162,     1,   165,     2,    41,     0,    30,
       0,     0,     0,   105,     0,     0,     0,     0,     0,    98,
      98,     0,     0,   115,   117,     0,   126,     0,     0,     0,
     126,     0,     0,     0,     0,    43,     0,     0,   119,   104,
       0,    97,   100,   101,   102,   103,    99,     0,     0,   123,
     122,   118,    76,     0,    75,   116,   128,    83,   114,    33,
       0,    68,   126,    70,     0,   163,     0,    46,     0,     0,
      40,    45,     0,    39,     0,   109,   110,     0,     0,   124,
       0,   141,     0,   127,   130,   129,   132,     0,     0,    81,
      98,    60,    69,     0,     0,     0,     0,     0,    54,    55,
      56,    57,    58,    50,    44,     0,   120,     0,   116,   142,
     143,   144,   145,   146,   147,   148,     0,   149,   151,     0,
     133,     0,     0,     0,   128,   128,   140,     0,   128,    85,
       0,     0,    59,    71,    72,     0,    46,    42,     0,     0,
      52,    49,     0,   116,   125,   150,   152,     0,   136,   138,
     139,   131,   134,   135,    93,    82,     0,    73,    62,    60,
     161,    47,    53,     0,    51,    37,     0,   137,     0,    84,
       0,    61,    50,     0,     0,   128,    77,    93,    86,    87,
      90,    48,    37,    34,    80,    94,     0,    92,    91,    89,
      38,    88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -203,  -203,   222,  -203,  -203,  -203,  -203,  -203,  -203,  -203,
    -203,  -203,  -203,  -203,  -203,   -26,  -203,  -203,  -203,  -203,
      18,    66,    -6,  -203,  -203,  -203,     8,    37,   137,  -203,
    -203,  -203,    77,  -203,   -45,  -203,  -203,  -203,  -203,  -203,
    -203,  -203,   -14,  -203,  -203,     3,  -203,     1,  -109,    -4,
     149,  -203,  -143,  -203,  -203,  -202,  -203,   -42,  -195,  -203,
    -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,  -203,
    -203,  -203,  -203,     0,  -203
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    44,   264,    32,    33,   125,   160,
     187,   157,   231,   253,   193,    34,   222,   181,    66,    67,
      35,    36,   152,   153,    37,   116,   142,   143,   266,   219,
     179,   247,   268,   269,   279,   259,    38,   136,   137,    69,
      78,    79,   113,    70,    71,    72,   144,   147,   173,   210,
     174,   175,   176,   177,   211,   212,   213,    73,    74,    39,
      40,    41,    96,    75,    76
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,   138,   169,   101,    68,    85,    47,   228,   214,     4,
      90,    86,    87,   237,   277,   244,    91,   238,   229,   242,
     243,    47,   230,   245,   107,   108,    42,    59,    49,    43,
      60,    61,    92,    63,   188,   189,   190,   191,   192,    50,
      47,    88,    97,    49,   102,    99,   100,   215,    80,    81,
      82,    83,    84,   229,    50,   234,   267,   230,   270,   278,
     103,   -78,    49,    80,    81,    82,    83,    84,   159,   112,
     274,   220,    62,    50,   270,   129,   130,   114,   151,    45,
     161,    46,    80,    81,    82,    83,    84,    62,   119,   120,
     256,   104,   139,    89,   235,    47,   236,    94,   126,    95,
      98,   132,   133,   134,   135,   131,    62,  -153,  -154,  -155,
     182,   140,   141,    77,   194,   145,  -156,    49,   149,  -157,
     109,   154,   110,   117,   158,   111,   122,   163,    50,   115,
     121,   164,   105,   106,   107,   108,   124,    80,    81,    82,
      83,    84,   172,   118,   127,   114,   123,   105,   106,   107,
     108,   105,   106,   107,   108,   128,   146,    47,   112,   150,
     156,    62,   195,   162,   105,   106,   107,   108,   198,   167,
     165,   166,   184,   216,   168,   180,   178,   185,   183,    49,
     224,   186,     4,   154,    47,   196,   158,   197,   217,   199,
      50,   218,   221,   225,   227,    48,   232,   233,   114,    80,
      81,    82,    83,    84,   241,   248,    49,   239,   240,   252,
     172,   172,   254,   258,   172,   246,   257,    50,   214,   260,
     112,   262,   265,    62,   263,   250,    51,    52,    53,    54,
      55,    47,   255,   114,   273,     4,    56,    57,    58,   276,
      93,   170,    48,   171,   251,    59,   280,   -79,    60,    61,
      62,    63,   226,    49,    64,    65,   271,   261,   249,   155,
     223,   172,   281,   272,    50,     0,   148,    47,     0,     0,
     275,     0,     0,    51,    52,    53,    54,    55,    48,     0,
       0,     0,     0,    56,    57,    58,     0,     0,     0,    49,
       0,     0,    59,     0,     0,    60,    61,    62,    63,     0,
      50,    64,    65,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,    55,     0,     0,     0,     0,     0,    56,
      57,    58,     0,     0,     0,     0,     0,     0,    59,     0,
       0,    60,    61,    62,    63,     1,     2,    64,    65,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,     0,     0,     0,
      14,    15,   200,   201,   202,   203,   204,   205,    16,     0,
      17,     0,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,   207,     0,   208,     0,   209,     0,
       0,     0,     0,     0,     0,   105,   106,   107,   108
};

static const yytype_int16 yycheck[] =
{
       4,   110,   145,    48,     3,     5,     7,    18,    36,    11,
      10,     7,     8,    64,    12,   217,    16,    68,    64,   214,
     215,     7,    68,   218,    78,    79,     6,    68,    29,     9,
      71,    72,    40,    74,    24,    25,    26,    27,    28,    40,
       7,    32,    42,    29,    48,    45,    46,    75,    49,    50,
      51,    52,    53,    64,    40,   198,   258,    68,   260,    57,
      64,    62,    29,    49,    50,    51,    52,    53,    70,    70,
     265,   180,    73,    40,   276,    19,    20,    77,   120,     6,
     125,     8,    49,    50,    51,    52,    53,    73,    88,    89,
     233,    20,    78,    34,    65,     7,    67,     0,    98,     3,
       8,   105,   106,   107,   108,   104,    73,    18,    18,    18,
     152,   111,   112,   117,   159,   115,    18,    29,   118,    18,
      18,   121,    18,    20,   124,    31,    43,   127,    40,    34,
      37,   130,    76,    77,    78,    79,    18,    49,    50,    51,
      52,    53,   146,    34,    38,   145,    41,    76,    77,    78,
      79,    76,    77,    78,    79,    19,    35,     7,    70,    33,
      74,    73,   162,    38,    76,    77,    78,    79,   168,    62,
      19,    19,    43,   177,    20,    18,    54,    32,    20,    29,
     184,    20,    11,   183,     7,    19,   186,    63,    56,    66,
      40,    58,    20,     6,    19,    18,    18,   197,   198,    49,
      50,    51,    52,    53,    74,    19,    29,   211,   212,    71,
     214,   215,    68,    20,   218,    55,    68,    40,    36,    56,
      70,    19,    38,    73,    20,   225,    49,    50,    51,    52,
      53,     7,   232,   233,    19,    11,    59,    60,    61,    20,
      18,    64,    18,    66,   226,    68,   272,    62,    71,    72,
      73,    74,   186,    29,    77,    78,   262,   249,   221,   122,
     183,   265,   276,   263,    40,    -1,   117,     7,    -1,    -1,
     267,    -1,    -1,    49,    50,    51,    52,    53,    18,    -1,
      -1,    -1,    -1,    59,    60,    61,    -1,    -1,    -1,    29,
      -1,    -1,    68,    -1,    -1,    71,    72,    73,    74,    -1,
      40,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    71,    72,    73,    74,     4,     5,    77,    78,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      -1,    -1,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      29,    30,    43,    44,    45,    46,    47,    48,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,    10,    11,    12,    13,    14,    15,    16,
      17,    21,    22,    23,    29,    30,    37,    39,    42,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    97,    98,   106,   111,   112,   115,   127,   150,
     151,   152,     6,     9,    95,     6,     8,     7,    18,    29,
      40,    49,    50,    51,    52,    53,    59,    60,    61,    68,
      71,    72,    73,    74,    77,    78,   109,   110,   128,   130,
     134,   135,   136,   148,   149,   154,   155,   130,   131,   132,
      49,    50,    51,    52,    53,   154,     7,     8,    32,    34,
     154,   154,    40,    83,     0,     3,   153,   154,     8,   154,
     154,   115,   130,   130,    20,    76,    77,    78,    79,    18,
      18,    31,    70,   133,   154,    34,   116,    20,    34,   154,
     154,    37,    43,    41,    18,    99,   154,    38,    19,    19,
      20,   128,   130,   130,   130,   130,   128,   129,   129,    78,
     154,   154,   117,   118,   137,   154,    35,   138,   131,   154,
      33,   138,   113,   114,   154,   109,    74,   102,   154,    70,
     100,   115,    38,   154,   128,    19,    19,    62,    20,   133,
      64,    66,   130,   139,   141,   142,   143,   144,    54,   121,
      18,   108,   138,    20,    43,    32,    20,   101,    24,    25,
      26,    27,    28,   105,   115,   154,    19,    63,   154,    66,
      43,    44,    45,    46,    47,    48,    64,    65,    67,    69,
     140,   145,   146,   147,    36,    75,   130,    56,    58,   120,
     129,    20,   107,   113,   130,     6,   102,    19,    18,    64,
      68,   103,    18,   154,   133,    65,    67,    64,    68,   130,
     130,    74,   139,   139,   136,   139,    55,   122,    19,   108,
     154,   101,    71,   104,    68,   154,   133,    68,    20,   126,
      56,   107,    19,    20,    96,    38,   119,   136,   123,   124,
     136,   103,   154,    19,   139,   126,    20,    12,    57,   125,
      96,   123
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    95,    96,    96,    97,
      98,    99,    99,   100,   100,   100,   101,   101,   102,   102,
     103,   103,   103,   104,   105,   105,   105,   105,   105,   106,
     107,   107,   108,   109,   109,   109,   109,   110,   111,   112,
     113,   113,   114,   115,   116,   116,   116,   117,   118,   118,
     119,   120,   120,   121,   121,   122,   122,   123,   123,   124,
     125,   125,   125,   126,   126,   127,   128,   128,   129,   129,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   131,   131,   132,   133,   133,   133,   134,
     135,   136,   136,   136,   137,   137,   138,   138,   139,   139,
     139,   139,   139,   139,   139,   139,   140,   140,   141,   142,
     143,   144,   144,   145,   145,   145,   145,   145,   145,   146,
     146,   147,   147,   148,   148,   148,   148,   148,   149,   149,
     149,   150,   151,   152,   153,   153,   154,   154,   155,   155,
     155,   155,   155,   155,   155,   155
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     4,    10,     0,     1,     0,     3,     5,
       5,     0,     4,     0,     2,     1,     0,     3,     6,     3,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     6,
       0,     3,     3,     1,     1,     1,     1,     1,     4,     5,
       1,     3,     3,     7,     0,     2,     2,     6,     1,     6,
       2,     0,     2,     0,     4,     0,     3,     1,     3,     2,
       0,     1,     1,     0,     3,     2,     1,     3,     0,     1,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     4,
       4,     1,     1,     1,     3,     2,     0,     1,     2,     3,
       5,     1,     3,     3,     2,     4,     0,     2,     0,     1,
       1,     3,     1,     2,     3,     3,     2,     3,     3,     3,
       2,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     7,     2,     4,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
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
#line 258 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1909 "yacc_sql.cpp"
    break;

  case 24: /* exit_stmt: EXIT  */
#line 289 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1918 "yacc_sql.cpp"
    break;

  case 25: /* help_stmt: HELP  */
#line 295 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1926 "yacc_sql.cpp"
    break;

  case 26: /* sync_stmt: SYNC  */
#line 300 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1934 "yacc_sql.cpp"
    break;

  case 27: /* begin_stmt: TRX_BEGIN  */
#line 306 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1942 "yacc_sql.cpp"
    break;

  case 28: /* commit_stmt: TRX_COMMIT  */
#line 312 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1950 "yacc_sql.cpp"
    break;

  case 29: /* rollback_stmt: TRX_ROLLBACK  */
#line 318 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1958 "yacc_sql.cpp"
    break;

  case 30: /* drop_table_stmt: DROP TABLE id  */
#line 324 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      auto *drop_table = new DropTableSqlNode;
      (yyval.sql_node)->node.drop_table = drop_table;
      drop_table->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1970 "yacc_sql.cpp"
    break;

  case 31: /* show_tables_stmt: SHOW TABLES  */
#line 333 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1978 "yacc_sql.cpp"
    break;

  case 32: /* desc_table_stmt: DESC id  */
#line 339 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      auto *desc_table = new DescTableSqlNode;
      (yyval.sql_node)->node.desc_table = desc_table;
      desc_table->relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1990 "yacc_sql.cpp"
    break;

  case 33: /* show_index_stmt: SHOW INDEX FROM id  */
#line 349 "yacc_sql.y"
                       {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      auto *show_index = new ShowIndexSqlNode;
      (yyval.sql_node)->node.show_index = show_index;
      show_index->table_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2002 "yacc_sql.cpp"
    break;

  case 34: /* create_index_stmt: CREATE unique INDEX id ON id LBRACE id ids RBRACE  */
#line 360 "yacc_sql.y"
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
#line 2022 "yacc_sql.cpp"
    break;

  case 35: /* unique: %empty  */
#line 378 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 2030 "yacc_sql.cpp"
    break;

  case 36: /* unique: UNIQUE  */
#line 381 "yacc_sql.y"
             {
      (yyval.bools) = true;
    }
#line 2038 "yacc_sql.cpp"
    break;

  case 37: /* ids: %empty  */
#line 386 "yacc_sql.y"
   {
      (yyval.id_list) = new std::vector<std::string>();
   }
#line 2046 "yacc_sql.cpp"
    break;

  case 38: /* ids: COMMA id ids  */
#line 389 "yacc_sql.y"
                  {
      (yyvsp[0].id_list)->push_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
      (yyval.id_list) = (yyvsp[0].id_list);
   }
#line 2056 "yacc_sql.cpp"
    break;

  case 39: /* drop_index_stmt: DROP INDEX id ON id  */
#line 397 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      auto *drop_index = new DropIndexSqlNode;
      (yyval.sql_node)->node.drop_index = drop_index;
      drop_index->index_name = (yyvsp[-2].string);
      drop_index->relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2070 "yacc_sql.cpp"
    break;

  case 40: /* create_table_stmt: CREATE TABLE id attr_list as_select  */
#line 410 "yacc_sql.y"
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
#line 2087 "yacc_sql.cpp"
    break;

  case 41: /* attr_list: %empty  */
#line 425 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2095 "yacc_sql.cpp"
    break;

  case 42: /* attr_list: LBRACE attr_def attr_def_list RBRACE  */
#line 428 "yacc_sql.y"
                                           {
      if ((yyvsp[-1].attr_infos) == nullptr) {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      } else {
        (yyval.attr_infos) = (yyvsp[-1].attr_infos);
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-2].attr_info));
      std::reverse((yyval.attr_infos)->begin(), (yyval.attr_infos)->end());
    }
#line 2109 "yacc_sql.cpp"
    break;

  case 43: /* as_select: %empty  */
#line 439 "yacc_sql.y"
    {
      (yyval.sql_node) = nullptr;
    }
#line 2117 "yacc_sql.cpp"
    break;

  case 44: /* as_select: AS select_stmt  */
#line 442 "yacc_sql.y"
                     {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2125 "yacc_sql.cpp"
    break;

  case 45: /* as_select: select_stmt  */
#line 445 "yacc_sql.y"
                  {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2133 "yacc_sql.cpp"
    break;

  case 46: /* attr_def_list: %empty  */
#line 451 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2141 "yacc_sql.cpp"
    break;

  case 47: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 455 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2155 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: id type LBRACE number RBRACE null_def  */
#line 468 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-5].string));
    }
#line 2168 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: id type null_def  */
#line 477 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-2].string));
    }
#line 2181 "yacc_sql.cpp"
    break;

  case 50: /* null_def: %empty  */
#line 488 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 2189 "yacc_sql.cpp"
    break;

  case 51: /* null_def: NOT NULL_V  */
#line 491 "yacc_sql.y"
                 {
      (yyval.bools) = false;
    }
#line 2197 "yacc_sql.cpp"
    break;

  case 52: /* null_def: NULL_V  */
#line 494 "yacc_sql.y"
             {
      (yyval.bools) = true;
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 53: /* number: NUMBER  */
#line 499 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2211 "yacc_sql.cpp"
    break;

  case 54: /* type: INT_T  */
#line 503 "yacc_sql.y"
               { (yyval.number)=INTS; }
#line 2217 "yacc_sql.cpp"
    break;

  case 55: /* type: STRING_T  */
#line 504 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 2223 "yacc_sql.cpp"
    break;

  case 56: /* type: FLOAT_T  */
#line 505 "yacc_sql.y"
               { (yyval.number)=FLOATS; }
#line 2229 "yacc_sql.cpp"
    break;

  case 57: /* type: DATE_T  */
#line 506 "yacc_sql.y"
               { (yyval.number)=DATES; }
#line 2235 "yacc_sql.cpp"
    break;

  case 58: /* type: TEXT_T  */
#line 507 "yacc_sql.y"
               { (yyval.number)=TEXTS; }
#line 2241 "yacc_sql.cpp"
    break;

  case 59: /* insert_stmt: INSERT INTO id VALUES record record_list  */
#line 512 "yacc_sql.y"
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
#line 2260 "yacc_sql.cpp"
    break;

  case 60: /* record_list: %empty  */
#line 529 "yacc_sql.y"
    {
      (yyval.record_list) = nullptr;
    }
#line 2268 "yacc_sql.cpp"
    break;

  case 61: /* record_list: COMMA record record_list  */
#line 532 "yacc_sql.y"
                               {
      if ((yyvsp[0].record_list) != nullptr) {
        (yyval.record_list) = (yyvsp[0].record_list);
      } else {
        (yyval.record_list) = new std::vector<std::vector<ExprSqlNode *>>;
      }
      (yyval.record_list)->emplace_back(*(yyvsp[-1].expression_list));
      delete (yyvsp[-1].expression_list);
    }
#line 2282 "yacc_sql.cpp"
    break;

  case 62: /* record: LBRACE expression_list_empty RBRACE  */
#line 544 "yacc_sql.y"
    {
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[-1].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      }
      reverse((yyval.expression_list)->begin(), (yyval.expression_list)->end());
    }
#line 2295 "yacc_sql.cpp"
    break;

  case 63: /* value: NUMBER  */
#line 554 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2304 "yacc_sql.cpp"
    break;

  case 64: /* value: FLOAT  */
#line 558 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2313 "yacc_sql.cpp"
    break;

  case 65: /* value: SSS  */
#line 562 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
    }
#line 2323 "yacc_sql.cpp"
    break;

  case 66: /* value: NULL_V  */
#line 567 "yacc_sql.y"
             {
      (yyval.value) = new Value;
      (yyval.value)->set_null();
    }
#line 2332 "yacc_sql.cpp"
    break;

  case 67: /* value_expr: value  */
#line 574 "yacc_sql.y"
          {
      (yyval.value_expr) = new ValueExprSqlNode;
      (yyval.value_expr)->value = *(yyvsp[0].value);
      delete (yyvsp[0].value);
    }
#line 2342 "yacc_sql.cpp"
    break;

  case 68: /* delete_stmt: DELETE FROM id where  */
#line 582 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      auto *deletion = new DeleteSqlNode;
      (yyval.sql_node)->node.deletion = deletion;
      deletion->relation_name = (yyvsp[-1].string);
      deletion->conditions = (yyvsp[0].conjunction);
      free((yyvsp[-1].string));
    }
#line 2355 "yacc_sql.cpp"
    break;

  case 69: /* update_stmt: UPDATE id SET update_set_list where  */
#line 594 "yacc_sql.y"
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
#line 2370 "yacc_sql.cpp"
    break;

  case 70: /* update_set_list: update_set  */
#line 608 "yacc_sql.y"
    {
      (yyval.update_set_list) = new std::vector<UpdateSetSqlNode *>(1, (yyvsp[0].update_set));
    }
#line 2378 "yacc_sql.cpp"
    break;

  case 71: /* update_set_list: update_set COMMA update_set_list  */
#line 611 "yacc_sql.y"
                                       {
      (yyval.update_set_list) = (yyvsp[0].update_set_list);
      (yyval.update_set_list)->push_back((yyvsp[-2].update_set));
    }
#line 2387 "yacc_sql.cpp"
    break;

  case 72: /* update_set: id EQ expression  */
#line 617 "yacc_sql.y"
                     {
      (yyval.update_set) = new UpdateSetSqlNode;
      (yyval.update_set)->field_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.update_set)->expr = (yyvsp[0].expression);
    }
#line 2398 "yacc_sql.cpp"
    break;

  case 73: /* select_stmt: SELECT select_attr_list from where groupby having orderby  */
#line 626 "yacc_sql.y"
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
    }
#line 2427 "yacc_sql.cpp"
    break;

  case 74: /* from: %empty  */
#line 653 "yacc_sql.y"
    {
      (yyval.join) = nullptr;
    }
#line 2435 "yacc_sql.cpp"
    break;

  case 75: /* from: FROM rel_list  */
#line 656 "yacc_sql.y"
                    {
      (yyval.join) = (yyvsp[0].join);
    }
#line 2443 "yacc_sql.cpp"
    break;

  case 76: /* from: FROM joined_tables  */
#line 659 "yacc_sql.y"
                         {
      (yyval.join) = (yyvsp[0].join);
    }
#line 2451 "yacc_sql.cpp"
    break;

  case 77: /* joined_tables: joined_tables_inner INNER JOIN id as_info joined_on  */
#line 665 "yacc_sql.y"
                                                        {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation=(yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.join)->alias = (yyvsp[-1].string);
      if(*(yyvsp[-1].string)) free((yyvsp[-1].string));
      (yyval.join)->sub_join=(yyvsp[-5].join);
      (yyval.join)->join_conditions=(yyvsp[0].conjunction);  
    }
#line 2465 "yacc_sql.cpp"
    break;

  case 78: /* joined_tables_inner: id  */
#line 676 "yacc_sql.y"
       {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2475 "yacc_sql.cpp"
    break;

  case 79: /* joined_tables_inner: joined_tables_inner INNER JOIN id as_info joined_on  */
#line 681 "yacc_sql.y"
                                                          {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation=(yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.join)->sub_join=(yyvsp[-5].join);
      (yyval.join)->alias=(yyvsp[-1].string);
      if(*(yyvsp[-1].string)) free((yyvsp[-1].string));
      (yyval.join)->join_conditions=(yyvsp[0].conjunction);  
    }
#line 2489 "yacc_sql.cpp"
    break;

  case 80: /* joined_on: ON conjunction  */
#line 692 "yacc_sql.y"
                   {
      (yyval.conjunction) = (yyvsp[0].conjunction);
    }
#line 2497 "yacc_sql.cpp"
    break;

  case 81: /* having: %empty  */
#line 697 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2505 "yacc_sql.cpp"
    break;

  case 82: /* having: HAVING conjunction  */
#line 700 "yacc_sql.y"
                         {
      (yyval.conjunction) = (yyvsp[0].conjunction);
    }
#line 2513 "yacc_sql.cpp"
    break;

  case 83: /* groupby: %empty  */
#line 706 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2521 "yacc_sql.cpp"
    break;

  case 84: /* groupby: GROUP BY rel_attr rel_attr_list  */
#line 710 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      if ((yyval.rel_attr_list) == nullptr) {
        (yyval.rel_attr_list) = new std::vector<FieldExprSqlNode *>();
      }
      (yyval.rel_attr_list)->push_back((yyvsp[-1].rel_attr));
      std::reverse((yyval.rel_attr_list)->begin(), (yyval.rel_attr_list)->end());
    }
#line 2534 "yacc_sql.cpp"
    break;

  case 85: /* orderby: %empty  */
#line 720 "yacc_sql.y"
    {
      (yyval.order_unit_list) = nullptr;
    }
#line 2542 "yacc_sql.cpp"
    break;

  case 86: /* orderby: ORDER BY order_unit_list  */
#line 723 "yacc_sql.y"
                               {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      std::reverse((yyval.order_unit_list)->begin(), (yyval.order_unit_list)->end());
    }
#line 2551 "yacc_sql.cpp"
    break;

  case 87: /* order_unit_list: order_unit  */
#line 730 "yacc_sql.y"
    {
      (yyval.order_unit_list) = new std::vector<OrderBySqlNode *>();
      (yyval.order_unit_list)->push_back((yyvsp[0].order_unit));
    }
#line 2560 "yacc_sql.cpp"
    break;

  case 88: /* order_unit_list: order_unit COMMA order_unit_list  */
#line 735 "yacc_sql.y"
    {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      (yyval.order_unit_list)->push_back((yyvsp[-2].order_unit));
    }
#line 2569 "yacc_sql.cpp"
    break;

  case 89: /* order_unit: rel_attr order  */
#line 741 "yacc_sql.y"
                   {
      (yyval.order_unit) = new OrderBySqlNode;
      (yyval.order_unit)->field = (yyvsp[-1].rel_attr);
      (yyval.order_unit)->order = (yyvsp[0].order);
    }
#line 2579 "yacc_sql.cpp"
    break;

  case 90: /* order: %empty  */
#line 748 "yacc_sql.y"
    {
      (yyval.order) = Order::ASC;
    }
#line 2587 "yacc_sql.cpp"
    break;

  case 91: /* order: ASC  */
#line 751 "yacc_sql.y"
          {
      (yyval.order) = Order::ASC;
    }
#line 2595 "yacc_sql.cpp"
    break;

  case 92: /* order: DESC  */
#line 754 "yacc_sql.y"
           {
      (yyval.order) = Order::DESC;
    }
#line 2603 "yacc_sql.cpp"
    break;

  case 93: /* rel_attr_list: %empty  */
#line 759 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = nullptr;
    }
#line 2611 "yacc_sql.cpp"
    break;

  case 94: /* rel_attr_list: COMMA rel_attr rel_attr_list  */
#line 763 "yacc_sql.y"
    {
      (yyval.rel_attr_list) = (yyvsp[0].rel_attr_list);
      if ((yyval.rel_attr_list) == nullptr) {
        (yyval.rel_attr_list) = new std::vector<FieldExprSqlNode *>();
      }
      (yyval.rel_attr_list)->push_back((yyvsp[-1].rel_attr));
    }
#line 2623 "yacc_sql.cpp"
    break;

  case 95: /* calc_stmt: CALC expression_list  */
#line 773 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      auto *tmp = new CalcSqlNode;
      (yyval.sql_node)->node.calc = tmp;
      std::reverse((yyvsp[0].expression_list)->begin(), (yyvsp[0].expression_list)->end());
      tmp->expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2636 "yacc_sql.cpp"
    break;

  case 96: /* expression_list: expression  */
#line 785 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2645 "yacc_sql.cpp"
    break;

  case 97: /* expression_list: expression COMMA expression_list  */
#line 790 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<ExprSqlNode *>;
      }
      (yyval.expression_list)->emplace_back((yyvsp[-2].expression));
    }
#line 2658 "yacc_sql.cpp"
    break;

  case 98: /* expression_list_empty: %empty  */
#line 801 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2666 "yacc_sql.cpp"
    break;

  case 99: /* expression_list_empty: expression_list  */
#line 804 "yacc_sql.y"
                      {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2674 "yacc_sql.cpp"
    break;

  case 100: /* expression: expression '+' expression  */
#line 809 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2682 "yacc_sql.cpp"
    break;

  case 101: /* expression: expression '-' expression  */
#line 812 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2690 "yacc_sql.cpp"
    break;

  case 102: /* expression: expression '*' expression  */
#line 815 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2698 "yacc_sql.cpp"
    break;

  case 103: /* expression: expression '/' expression  */
#line 818 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2706 "yacc_sql.cpp"
    break;

  case 104: /* expression: LBRACE expression RBRACE  */
#line 821 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2715 "yacc_sql.cpp"
    break;

  case 105: /* expression: '-' expression  */
#line 825 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticType::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2723 "yacc_sql.cpp"
    break;

  case 106: /* expression: '*'  */
#line 828 "yacc_sql.y"
          {
      (yyval.expression) = new ExprSqlNode(new StarExprSqlNode);
    }
#line 2731 "yacc_sql.cpp"
    break;

  case 107: /* expression: rel_attr  */
#line 831 "yacc_sql.y"
               {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].rel_attr));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2740 "yacc_sql.cpp"
    break;

  case 108: /* expression: value_expr  */
#line 835 "yacc_sql.y"
                 {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].value_expr));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2749 "yacc_sql.cpp"
    break;

  case 109: /* expression: aggr_op LBRACE expression_list_empty RBRACE  */
#line 839 "yacc_sql.y"
                                                  {
      std::string name = token_name(sql_string, &(yyloc));
      if ((yyvsp[-1].expression_list)) {
        (yyval.expression) = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode((yyvsp[-3].aggr), *(yyvsp[-1].expression_list))));
      } else {
        (yyval.expression) = new ExprSqlNode(new NamedExprSqlNode(name, new AggregationExprSqlNode((yyvsp[-3].aggr))));
      }
      (yyval.expression)->set_name(name);
    }
#line 2763 "yacc_sql.cpp"
    break;

  case 110: /* expression: func_op LBRACE expression_list_empty RBRACE  */
#line 848 "yacc_sql.y"
                                                  {
      std::string name = token_name(sql_string, &(yyloc));
      reverse((yyvsp[-1].expression_list)->begin(), (yyvsp[-1].expression_list)->end());
      (yyval.expression) = new ExprSqlNode(new FunctionExprSqlNode((yyvsp[-3].func), (yyvsp[-1].expression_list)));
      delete (yyvsp[-1].expression_list);
      (yyval.expression)->set_name(name);
    }
#line 2775 "yacc_sql.cpp"
    break;

  case 111: /* expression: list_expr  */
#line 855 "yacc_sql.y"
                {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].list));
      std::string name = token_name(sql_string, &(yyloc));
      (yyval.expression)->set_name(name);
    }
#line 2785 "yacc_sql.cpp"
    break;

  case 112: /* expression: set_expr  */
#line 860 "yacc_sql.y"
               {
      (yyval.expression) = new ExprSqlNode((yyvsp[0].set));
      std::string name = token_name(sql_string, &(yyloc));
      (yyval.expression)->set_name(name);
    }
#line 2795 "yacc_sql.cpp"
    break;

  case 113: /* select_attr_list: select_attr  */
#line 868 "yacc_sql.y"
                {
      (yyval.select_attr_list) = new std::vector<SelectAttribute *>(1, (yyvsp[0].select_attr));
    }
#line 2803 "yacc_sql.cpp"
    break;

  case 114: /* select_attr_list: select_attr COMMA select_attr_list  */
#line 871 "yacc_sql.y"
                                         {
      (yyvsp[0].select_attr_list)->push_back((yyvsp[-2].select_attr));
      (yyval.select_attr_list) = (yyvsp[0].select_attr_list);
    }
#line 2812 "yacc_sql.cpp"
    break;

  case 115: /* select_attr: expression as_info  */
#line 878 "yacc_sql.y"
                       {
      (yyval.select_attr) = new SelectAttribute;
      (yyval.select_attr)->expr = (yyvsp[-1].expression);
      (yyval.select_attr)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
    }
#line 2823 "yacc_sql.cpp"
    break;

  case 116: /* as_info: %empty  */
#line 886 "yacc_sql.y"
    {
      (yyval.string) = "";
    }
#line 2831 "yacc_sql.cpp"
    break;

  case 117: /* as_info: id  */
#line 889 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2839 "yacc_sql.cpp"
    break;

  case 118: /* as_info: AS id  */
#line 892 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2847 "yacc_sql.cpp"
    break;

  case 119: /* list_expr: LBRACE select_stmt RBRACE  */
#line 897 "yacc_sql.y"
                              {
      (yyval.list) = new ListExprSqlNode((yyvsp[-1].sql_node)->node.selection);
      (yyvsp[-1].sql_node)->node.selection = nullptr;
      delete (yyvsp[-1].sql_node);
    }
#line 2857 "yacc_sql.cpp"
    break;

  case 120: /* set_expr: LBRACE expression COMMA expression_list RBRACE  */
#line 905 "yacc_sql.y"
                                                   {
      (yyvsp[-1].expression_list)->push_back((yyvsp[-3].expression));
      (yyval.set) = new SetExprSqlNode();
      (yyval.set)->expressions.swap(*(yyvsp[-1].expression_list));
      delete (yyvsp[-1].expression_list);
    }
#line 2868 "yacc_sql.cpp"
    break;

  case 121: /* rel_attr: id  */
#line 914 "yacc_sql.y"
       {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->field_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2878 "yacc_sql.cpp"
    break;

  case 122: /* rel_attr: id DOT id  */
#line 919 "yacc_sql.y"
                {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->table_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->field_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2890 "yacc_sql.cpp"
    break;

  case 123: /* rel_attr: id DOT '*'  */
#line 926 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new FieldExprSqlNode;
      (yyval.rel_attr)->table_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->field_name = "*";
      free((yyvsp[-2].string));
    }
#line 2901 "yacc_sql.cpp"
    break;

  case 124: /* rel_list: id as_info  */
#line 936 "yacc_sql.y"
               {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[-1].string);
      (yyval.join)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
      free((yyvsp[-1].string));
    }
#line 2913 "yacc_sql.cpp"
    break;

  case 125: /* rel_list: rel_list COMMA id as_info  */
#line 943 "yacc_sql.y"
                                {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[-1].string);
      free((yyvsp[-1].string));
      (yyval.join)->alias = (yyvsp[0].string);
      if(*(yyvsp[0].string)) free((yyvsp[0].string));
      (yyval.join)->sub_join = (yyvsp[-3].join);
    }
#line 2926 "yacc_sql.cpp"
    break;

  case 126: /* where: %empty  */
#line 955 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2934 "yacc_sql.cpp"
    break;

  case 127: /* where: WHERE conjunction  */
#line 958 "yacc_sql.y"
                        {
      (yyval.conjunction) = (yyvsp[0].conjunction);  
    }
#line 2942 "yacc_sql.cpp"
    break;

  case 128: /* conjunction: %empty  */
#line 964 "yacc_sql.y"
    {
      (yyval.conjunction) = nullptr;
    }
#line 2950 "yacc_sql.cpp"
    break;

  case 129: /* conjunction: contain  */
#line 967 "yacc_sql.y"
              {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].contain), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2958 "yacc_sql.cpp"
    break;

  case 130: /* conjunction: condition  */
#line 970 "yacc_sql.y"
                {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].condition), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2966 "yacc_sql.cpp"
    break;

  case 131: /* conjunction: expression like_op SSS  */
#line 973 "yacc_sql.y"
                             {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new LikeExprSqlNode((yyvsp[-1].bools), (yyvsp[-2].expression), (yyvsp[0].string)), static_cast<ExprSqlNode *>(nullptr));
      free((yyvsp[0].string));
    }
#line 2975 "yacc_sql.cpp"
    break;

  case 132: /* conjunction: exists  */
#line 977 "yacc_sql.y"
             {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, (yyvsp[0].exists), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2983 "yacc_sql.cpp"
    break;

  case 133: /* conjunction: expression null_check  */
#line 980 "yacc_sql.y"
                            {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::SINGLE, new NullCheckExprSqlNode((yyvsp[0].bools), (yyvsp[-1].expression)), static_cast<ExprSqlNode *>(nullptr));
    }
#line 2991 "yacc_sql.cpp"
    break;

  case 134: /* conjunction: conjunction AND conjunction  */
#line 983 "yacc_sql.y"
                                  {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::AND, (yyvsp[-2].conjunction), (yyvsp[0].conjunction));
    }
#line 2999 "yacc_sql.cpp"
    break;

  case 135: /* conjunction: conjunction OR conjunction  */
#line 986 "yacc_sql.y"
                                 {
      (yyval.conjunction) = new ConjunctionExprSqlNode(ConjunctionType::OR, (yyvsp[-2].conjunction), (yyvsp[0].conjunction));
    }
#line 3007 "yacc_sql.cpp"
    break;

  case 136: /* null_check: IS NULL_V  */
#line 992 "yacc_sql.y"
              {
      (yyval.bools) = true;
    }
#line 3015 "yacc_sql.cpp"
    break;

  case 137: /* null_check: IS NOT NULL_V  */
#line 995 "yacc_sql.y"
                    {
      (yyval.bools) = false;
    }
#line 3023 "yacc_sql.cpp"
    break;

  case 138: /* condition: expression comp_op expression  */
#line 1000 "yacc_sql.y"
                                  {
      (yyval.condition) = new ComparisonExprSqlNode((yyvsp[-1].comp), (yyvsp[-2].expression), (yyvsp[0].expression)); 
    }
#line 3031 "yacc_sql.cpp"
    break;

  case 139: /* contain: expression contain_op expression  */
#line 1006 "yacc_sql.y"
                                     {
      (yyval.contain) = new ContainExprSqlNode((yyvsp[-1].contain_op), (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 3039 "yacc_sql.cpp"
    break;

  case 140: /* exists: exists_op expression  */
#line 1012 "yacc_sql.y"
                         {
      (yyval.exists) = new ExistsExprSqlNode((yyvsp[-1].bools), (yyvsp[0].expression));
    }
#line 3047 "yacc_sql.cpp"
    break;

  case 141: /* exists_op: EXISTS  */
#line 1017 "yacc_sql.y"
           {
      (yyval.bools) = true;
    }
#line 3055 "yacc_sql.cpp"
    break;

  case 142: /* exists_op: NOT EXISTS  */
#line 1020 "yacc_sql.y"
                 {
      (yyval.bools) = false;
    }
#line 3063 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: EQ  */
#line 1025 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3069 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: LT  */
#line 1026 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3075 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: GT  */
#line 1027 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3081 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: LE  */
#line 1028 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3087 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: GE  */
#line 1029 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3093 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: NE  */
#line 1030 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3099 "yacc_sql.cpp"
    break;

  case 149: /* contain_op: IN  */
#line 1034 "yacc_sql.y"
         { (yyval.contain_op) = ContainType::IN; }
#line 3105 "yacc_sql.cpp"
    break;

  case 150: /* contain_op: NOT IN  */
#line 1035 "yacc_sql.y"
             { (yyval.contain_op) = ContainType::NOT_IN; }
#line 3111 "yacc_sql.cpp"
    break;

  case 151: /* like_op: LIKE  */
#line 1038 "yacc_sql.y"
           { (yyval.bools) = true; }
#line 3117 "yacc_sql.cpp"
    break;

  case 152: /* like_op: NOT LIKE  */
#line 1039 "yacc_sql.y"
               { (yyval.bools) = false; }
#line 3123 "yacc_sql.cpp"
    break;

  case 153: /* aggr_op: MIN  */
#line 1042 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_MIN; }
#line 3129 "yacc_sql.cpp"
    break;

  case 154: /* aggr_op: MAX  */
#line 1043 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_MAX; }
#line 3135 "yacc_sql.cpp"
    break;

  case 155: /* aggr_op: AVG  */
#line 1044 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_AVG; }
#line 3141 "yacc_sql.cpp"
    break;

  case 156: /* aggr_op: SUM  */
#line 1045 "yacc_sql.y"
          { (yyval.aggr) = AggregationType::AGGR_SUM; }
#line 3147 "yacc_sql.cpp"
    break;

  case 157: /* aggr_op: COUNT  */
#line 1046 "yacc_sql.y"
            { (yyval.aggr) = AggregationType::AGGR_COUNT; }
#line 3153 "yacc_sql.cpp"
    break;

  case 158: /* func_op: LENGTH  */
#line 1049 "yacc_sql.y"
             { (yyval.func) = FunctionType::LENGTH; }
#line 3159 "yacc_sql.cpp"
    break;

  case 159: /* func_op: ROUND  */
#line 1050 "yacc_sql.y"
            { (yyval.func) = FunctionType::ROUND; }
#line 3165 "yacc_sql.cpp"
    break;

  case 160: /* func_op: DATE_FORMAT  */
#line 1051 "yacc_sql.y"
                  { (yyval.func) = FunctionType::DATE_FORMAT; }
#line 3171 "yacc_sql.cpp"
    break;

  case 161: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE id  */
#line 1055 "yacc_sql.y"
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
#line 3187 "yacc_sql.cpp"
    break;

  case 162: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1070 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->node.explain = new ExplainSqlNode;
      (yyval.sql_node)->node.explain->sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3197 "yacc_sql.cpp"
    break;

  case 163: /* set_variable_stmt: SET id EQ value  */
#line 1079 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      auto *set_variable = new SetVariableSqlNode;
      (yyval.sql_node)->node.set_variable = set_variable;
      set_variable->name  = (yyvsp[-2].string);
      set_variable->value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3211 "yacc_sql.cpp"
    break;

  case 166: /* id: non_reserve  */
#line 1095 "yacc_sql.y"
                {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3219 "yacc_sql.cpp"
    break;

  case 167: /* id: ID  */
#line 1098 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3227 "yacc_sql.cpp"
    break;

  case 168: /* non_reserve: TABLES  */
#line 1103 "yacc_sql.y"
           {
      (yyval.string) = strdup("tables");
    }
#line 3235 "yacc_sql.cpp"
    break;

  case 169: /* non_reserve: HELP  */
#line 1106 "yacc_sql.y"
           {
      (yyval.string) = strdup("help");
    }
#line 3243 "yacc_sql.cpp"
    break;

  case 170: /* non_reserve: DATA  */
#line 1109 "yacc_sql.y"
           {
      (yyval.string) = strdup("data");
    }
#line 3251 "yacc_sql.cpp"
    break;

  case 171: /* non_reserve: MIN  */
#line 1112 "yacc_sql.y"
          {
      (yyval.string) = strdup("min");
    }
#line 3259 "yacc_sql.cpp"
    break;

  case 172: /* non_reserve: MAX  */
#line 1115 "yacc_sql.y"
          {
      (yyval.string) = strdup("max");
    }
#line 3267 "yacc_sql.cpp"
    break;

  case 173: /* non_reserve: AVG  */
#line 1118 "yacc_sql.y"
          {
      (yyval.string) = strdup("avg");
    }
#line 3275 "yacc_sql.cpp"
    break;

  case 174: /* non_reserve: SUM  */
#line 1121 "yacc_sql.y"
          {
      (yyval.string) = strdup("sum");
    }
#line 3283 "yacc_sql.cpp"
    break;

  case 175: /* non_reserve: COUNT  */
#line 1124 "yacc_sql.y"
            {
      (yyval.string) = strdup("count");
    }
#line 3291 "yacc_sql.cpp"
    break;


#line 3295 "yacc_sql.cpp"

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

#line 1128 "yacc_sql.y"

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
