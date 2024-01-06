/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NOT = 258,
     LP = 259,
     RP = 260,
     FnRetTypeDecl = 261,
     LET = 262,
     FN = 263,
     IF = 264,
     ELSE = 265,
     WHILE = 266,
     RET_ = 267,
     STRUCT = 268,
     BREAK = 269,
     CONTINUE = 270,
     TypeAssign = 271,
     StmtEnd = 272,
     EQ = 273,
     MLP = 274,
     MRP = 275,
     BLP = 276,
     BRP = 277,
     Comma = 278,
     POINT = 279,
     term = 280,
     LT = 281,
     LE = 282,
     GT = 283,
     GE = 284,
     EEQ = 285,
     NE = 286,
     OR = 287,
     AND = 288,
     ADD = 289,
     SUB = 290,
     DIV = 291,
     MUL = 292,
     NUMBER = 293,
     NativeType = 294
   };
#endif
/* Tokens.  */
#define NOT 258
#define LP 259
#define RP 260
#define FnRetTypeDecl 261
#define LET 262
#define FN 263
#define IF 264
#define ELSE 265
#define WHILE 266
#define RET_ 267
#define STRUCT 268
#define BREAK 269
#define CONTINUE 270
#define TypeAssign 271
#define StmtEnd 272
#define EQ 273
#define MLP 274
#define MRP 275
#define BLP 276
#define BRP 277
#define Comma 278
#define POINT 279
#define term 280
#define LT 281
#define LE 282
#define GT 283
#define GE 284
#define EEQ 285
#define NE 286
#define OR 287
#define AND 288
#define ADD 289
#define SUB 290
#define DIV 291
#define MUL 292
#define NUMBER 293
#define NativeType 294




/* Copy the first part of user declarations.  */
#line 1 "parser.yacc"

#include <stdio.h>
#include "TeaplAst.h"

//extern A_pos pos;
extern int line, col;
extern A_program root;
A_pos my_pos();
extern int yylex(void);
extern "C"{
extern void yyerror(char *s); 
extern int  yywrap();
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 20 "parser.yacc"
{
	//int val;
	//char *id;
	A_pos pos;
	A_tokenId tokenId;
	A_tokenNum tokenNum;

	A_type type;
	A_programElementList programElements;
	A_programElement programElement;

	A_varDeclStmt varDeclStmt;
	A_varDecl varDecl;
	A_varDef varDef;

	A_rightValList rightValList;

	A_varDeclList paramDecl;
	A_codeBlockStmtList stmts;

	A_structDef structDef;


	A_fnDeclStmt fnDeclStmt;
	A_fnDecl fnDecl;

	A_fnDef fnDef;

	A_codeBlockStmt codeBlockStmt;

	A_assignStmt assignStmt;
	A_callStmt callStmt;
	A_ifStmt ifStmt;
	A_whileStmt whileStmt;
	A_returnStmt returnStmt;

	//rightVal
	A_rightVal rightVal;
	//leftVal
	A_leftVal leftVal;
	A_arithExpr arithExpr;
	A_boolExpr boolExpr;
	A_arithBiOpExpr arithBiOpExpr;
	A_exprUnit exprUnit;
	A_arithUExpr arithUExpr;
	A_arithBiOp op;
	A_fnCall fnCall;
	A_indexExpr indexExpr;
	A_memberExpr memberExpr;
	A_boolBiOpExpr boolBiOpExpr;
	A_boolUnit boolUnit;
	A_comExpr comExpr;
	A_boolUOpExpr boolUOpExpr;


}
/* Line 193 of yacc.c.  */
#line 247 "y.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 260 "y.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   200

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  88
/* YYNRULES -- Number of states.  */
#define YYNSTATES  176

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      21,    25,    29,    31,    38,    43,    49,    53,    64,    73,
      74,    76,    78,    82,    86,    92,    98,    99,   102,   104,
     109,   111,   116,   124,   132,   144,   146,   148,   150,   153,
     155,   157,   161,   164,   167,   170,   172,   178,   183,   184,
     186,   188,   192,   195,   197,   199,   201,   203,   205,   207,
     211,   215,   219,   223,   225,   227,   231,   233,   238,   240,
     242,   247,   250,   254,   256,   258,   262,   266,   270,   274,
     278,   282,   284,   286,   290,   294,   296,   300,   302
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    -1,    42,    41,    -1,    43,    -1,    49,
      -1,    48,    -1,    50,    -1,    17,    -1,     7,    44,    17,
      -1,     7,    45,    17,    -1,    25,    16,    62,    -1,    25,
      -1,    25,    19,    38,    20,    16,    62,    -1,    25,    19,
      38,    20,    -1,    25,    16,    62,    18,    63,    -1,    25,
      18,    63,    -1,    25,    19,    38,    20,    16,    62,    18,
      21,    46,    22,    -1,    25,    19,    38,    20,    18,    21,
      46,    22,    -1,    -1,    47,    -1,    63,    -1,    63,    23,
      47,    -1,     8,    58,    17,    -1,    13,    25,    21,    60,
      22,    -1,     8,    58,    21,    51,    22,    -1,    -1,    56,
      51,    -1,    25,    -1,    52,    19,    70,    20,    -1,    69,
      -1,    52,    18,    63,    17,    -1,    11,     4,    57,     5,
      21,    51,    22,    -1,     9,     4,    57,     5,    21,    51,
      22,    -1,     9,     4,    57,     5,    21,    51,    22,    10,
      21,    51,    22,    -1,    17,    -1,    43,    -1,    53,    -1,
      67,    17,    -1,    55,    -1,    54,    -1,    12,    63,    17,
      -1,    12,    17,    -1,    15,    17,    -1,    14,    17,    -1,
      72,    -1,    25,     4,    59,     5,    61,    -1,    25,     4,
      59,     5,    -1,    -1,    60,    -1,    44,    -1,    44,    23,
      60,    -1,     6,    62,    -1,    25,    -1,    39,    -1,    64,
      -1,    72,    -1,    65,    -1,    66,    -1,    64,    35,    64,
      -1,    64,    34,    64,    -1,    64,    37,    64,    -1,    64,
      36,    64,    -1,    38,    -1,    25,    -1,     4,    64,     5,
      -1,    67,    -1,    52,    19,    70,    20,    -1,    69,    -1,
      68,    -1,    25,     4,    46,     5,    -1,    35,    66,    -1,
      52,    24,    25,    -1,    38,    -1,    25,    -1,    66,    26,
      66,    -1,    66,    27,    66,    -1,    66,    28,    66,    -1,
      66,    29,    66,    -1,    66,    30,    66,    -1,    66,    31,
      66,    -1,    73,    -1,    74,    -1,    72,    33,    72,    -1,
      72,    32,    72,    -1,    71,    -1,     4,    72,     5,    -1,
      75,    -1,     3,    74,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   134,   134,   135,   141,   144,   147,   150,   153,   157,
     158,   160,   164,   168,   172,   177,   181,   185,   188,   192,
     193,   198,   201,   205,   210,   214,   220,   221,   226,   227,
     228,   231,   236,   242,   246,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   265,   267,   270,   274,   275,
     280,   283,   287,   292,   295,   300,   301,   303,   304,   306,
     307,   308,   309,   311,   312,   313,   314,   315,   316,   317,
     319,   321,   324,   326,   327,   329,   330,   331,   332,   333,
     334,   336,   337,   339,   340,   342,   343,   344,   346
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NOT", "LP", "RP", "FnRetTypeDecl",
  "LET", "FN", "IF", "ELSE", "WHILE", "RET_", "STRUCT", "BREAK",
  "CONTINUE", "TypeAssign", "StmtEnd", "EQ", "MLP", "MRP", "BLP", "BRP",
  "Comma", "POINT", "term", "LT", "LE", "GT", "GE", "EEQ", "NE", "OR",
  "AND", "ADD", "SUB", "DIV", "MUL", "NUMBER", "NativeType", "$accept",
  "my_program", "my_programElement", "my_varDeclStmt", "my_varDecl",
  "my_varDef", "my_rightValList", "my_NotNullRightValList",
  "my_fnDeclStmt", "my_structDef", "my_fnDef", "my_codeBlockStmtList",
  "my_leftVal", "my_assignStmt", "my_whileStmt", "my_ifStmt",
  "my_codeBlockStmt", "my_ifWhileBoolExpr", "my_fnDecl", "my_ParamDecl",
  "my_NotNullParaDecl", "my_fnRetType", "Type", "my_rightVal",
  "my_arithExpr", "my_arithBiOpExpr", "my_exprUnit", "my_fnCall",
  "my_arithUExpr", "my_memberExpr", "my_indexExpr", "my_comExpr",
  "my_boolExpr", "my_boolBiOpExpr", "my_boolUnit", "my_boolUOpExpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    42,    42,    42,    43,
      43,    44,    44,    44,    44,    45,    45,    45,    45,    46,
      46,    47,    47,    48,    49,    50,    51,    51,    52,    52,
      52,    53,    54,    55,    55,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    57,    58,    58,    59,    59,
      60,    60,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    65,    65,    66,    66,    66,    66,    66,    66,    66,
      67,    68,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    72,    72,    73,    73,    74,    74,    74,    75
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     3,
       3,     3,     1,     6,     4,     5,     3,    10,     8,     0,
       1,     1,     3,     3,     5,     5,     0,     2,     1,     4,
       1,     4,     7,     7,    11,     1,     1,     1,     2,     1,
       1,     3,     2,     2,     2,     1,     5,     4,     0,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     1,     1,     3,     1,     4,     1,     1,
       4,     2,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     1,     3,     3,     1,     3,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     8,     0,     2,     4,     6,     5,
       7,    12,     0,     0,     0,     0,     0,     1,     3,     0,
       0,     0,     9,    10,    48,    23,    26,     0,    53,    54,
      11,     0,     0,    64,     0,    63,     0,    16,    55,    57,
      58,    66,    69,    68,    85,    56,    81,    82,    87,     0,
      12,    50,     0,    49,     0,     0,     0,     0,     0,    35,
      28,    36,     0,     0,    37,    40,    39,    26,     0,    30,
       0,     0,     0,    88,     0,     0,    19,     0,    71,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,    47,     0,     0,
      42,     0,    44,    43,    25,     0,     0,    27,    38,    24,
      15,    65,    86,     0,    20,    21,    58,    74,    73,     0,
      72,    60,    59,    62,    61,    75,    76,    77,    78,    79,
      80,    84,    83,     0,     0,    11,     0,    51,     0,    46,
       0,    45,     0,    41,     0,     0,    70,     0,    67,    13,
      19,    14,    52,     0,     0,    31,    29,    22,     0,     0,
       0,    26,    26,    19,    18,    13,     0,     0,     0,    33,
      32,    17,     0,    26,     0,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,    61,    51,    13,   113,   114,     8,     9,
      10,    62,    36,    64,    65,    66,    67,   140,    15,    52,
      53,   139,    30,   115,    38,    39,    40,    41,    42,    43,
     119,    44,    45,    46,    47,    48
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -133
static const yytype_int16 yypact[] =
{
     112,    -6,     2,    68,  -133,    94,   112,  -133,  -133,  -133,
    -133,    81,    85,    92,   113,    39,    77,  -133,  -133,   -10,
      17,    84,  -133,  -133,    98,  -133,   101,    98,  -133,  -133,
     109,    17,    17,     4,    76,  -133,    29,  -133,     1,  -133,
     115,  -133,  -133,    62,  -133,    26,  -133,  -133,  -133,   108,
     105,   110,   126,  -133,   130,   144,     9,   133,   135,  -133,
     149,  -133,   132,     6,  -133,  -133,  -133,   101,   138,  -133,
     134,    17,   115,  -133,    69,     0,    17,    76,  -133,   -16,
     136,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    17,    17,   114,   -10,   119,    98,   152,    17,    17,
    -133,   142,  -133,  -133,  -133,    17,   -16,  -133,  -133,  -133,
    -133,  -133,  -133,   155,  -133,   139,  -133,  -133,  -133,   143,
    -133,    13,    13,  -133,  -133,  -133,  -133,  -133,  -133,  -133,
    -133,   131,  -133,   -10,   145,  -133,   147,  -133,   -10,  -133,
     160,    26,   163,  -133,   153,   151,  -133,    17,    63,   154,
      17,   157,  -133,   148,   156,  -133,  -133,  -133,   158,   159,
     -10,   101,   101,    17,  -133,  -133,   161,   162,   164,   165,
    -133,  -133,   166,   101,   167,  -133
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,   168,  -133,    10,   175,  -133,  -132,    31,  -133,  -133,
    -133,   -66,   -26,  -133,  -133,  -133,  -133,    83,  -133,  -133,
     -21,  -133,   -87,   -17,     8,  -133,   -20,   -24,  -133,   -22,
      74,  -133,   -15,  -133,   169,  -133
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -31
static const yytype_int16 yytable[] =
{
      63,   107,    68,    37,    69,   112,    70,   135,    76,   117,
       7,    72,    31,    32,    78,    28,     7,    75,   159,    11,
      31,    32,   118,   -28,   105,   106,   100,    14,   -28,    29,
      80,   168,    91,    92,    33,    81,    82,    83,    84,   101,
      74,    63,    33,    68,    34,    69,   149,    35,    79,    83,
      84,   152,    34,    80,   110,    35,    25,   116,    91,    92,
      26,   116,   116,   116,   116,   125,   126,   127,   128,   129,
     130,    72,    72,   165,   111,   137,   131,   132,    72,    72,
      77,   -30,   -29,   141,   141,    74,   -30,   -29,   144,   121,
     122,   123,   124,    16,    17,   166,   167,    19,    27,    20,
      21,    33,    22,    81,    82,    83,    84,   174,     1,    23,
      54,    34,    55,    56,    35,    57,    58,    24,    59,     1,
       2,    94,    49,    50,    95,     3,    60,    71,    93,     4,
     133,    97,   134,    96,    98,    63,    63,    68,    68,    69,
      69,    85,    86,    87,    88,    89,    90,    63,    99,    68,
     102,    69,   103,    76,   104,   108,   109,   136,   138,   143,
     146,   120,   147,   148,    92,   153,   150,   151,   154,   161,
     155,   156,   158,   160,    18,   172,    12,   162,   157,   163,
     145,   164,   142,   169,   170,     0,   171,   173,     0,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      73
};

static const yytype_int16 yycheck[] =
{
      26,    67,    26,    20,    26,     5,    27,    94,     4,    25,
       0,    31,     3,     4,    34,    25,     6,    32,   150,    25,
       3,     4,    38,    19,    18,    19,    17,    25,    24,    39,
      24,   163,    32,    33,    25,    34,    35,    36,    37,    56,
      32,    67,    25,    67,    35,    67,   133,    38,    19,    36,
      37,   138,    35,    24,    71,    38,    17,    77,    32,    33,
      21,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   160,     5,    96,    91,    92,    98,    99,
       4,    19,    19,    98,    99,    77,    24,    24,   105,    81,
      82,    83,    84,    25,     0,   161,   162,    16,    21,    18,
      19,    25,    17,    34,    35,    36,    37,   173,     7,    17,
       9,    35,    11,    12,    38,    14,    15,     4,    17,     7,
       8,    16,    38,    25,    19,    13,    25,    18,    20,    17,
      16,     5,    18,    23,     4,   161,   162,   161,   162,   161,
     162,    26,    27,    28,    29,    30,    31,   173,     4,   173,
      17,   173,    17,     4,    22,    17,    22,    38,     6,    17,
       5,    25,    23,    20,    33,     5,    21,    20,     5,    21,
      17,    20,    18,    16,     6,    10,     1,    21,   147,    21,
     106,    22,    99,    22,    22,    -1,    22,    21,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,     8,    13,    17,    41,    42,    43,    48,    49,
      50,    25,    44,    45,    25,    58,    25,     0,    41,    16,
      18,    19,    17,    17,     4,    17,    21,    21,    25,    39,
      62,     3,     4,    25,    35,    38,    52,    63,    64,    65,
      66,    67,    68,    69,    71,    72,    73,    74,    75,    38,
      25,    44,    59,    60,     9,    11,    12,    14,    15,    17,
      25,    43,    51,    52,    53,    54,    55,    56,    67,    69,
      60,    18,    66,    74,    64,    72,     4,     4,    66,    19,
      24,    34,    35,    36,    37,    26,    27,    28,    29,    30,
      31,    32,    33,    20,    16,    19,    23,     5,     4,     4,
      17,    63,    17,    17,    22,    18,    19,    51,    17,    22,
      63,     5,     5,    46,    47,    63,    66,    25,    38,    70,
      25,    64,    64,    64,    64,    66,    66,    66,    66,    66,
      66,    72,    72,    16,    18,    62,    38,    60,     6,    61,
      57,    72,    57,    17,    63,    70,     5,    23,    20,    62,
      21,    20,    62,     5,     5,    17,    20,    47,    18,    46,
      16,    21,    21,    21,    22,    62,    51,    51,    46,    22,
      22,    22,    10,    21,    51,    22
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 134 "parser.yacc"
    {(yyval.programElements)=0; }
    break;

  case 3:
#line 135 "parser.yacc"
    {
		root = A_Program(A_ProgramElementList((yyvsp[(1) - (2)].programElement),(yyvsp[(2) - (2)].programElements)));
		(yyval.programElements)=root->programElements; 
	}
    break;

  case 4:
#line 141 "parser.yacc"
    { 
		(yyval.programElement)= A_ProgramVarDeclStmt((yyvsp[(1) - (1)].varDeclStmt)->pos,(yyvsp[(1) - (1)].varDeclStmt));
	}
    break;

  case 5:
#line 144 "parser.yacc"
    {
		(yyval.programElement)= A_ProgramStructDef((yyvsp[(1) - (1)].structDef)->pos,(yyvsp[(1) - (1)].structDef));
	}
    break;

  case 6:
#line 147 "parser.yacc"
    {
		(yyval.programElement)= A_ProgramFnDeclStmt((yyvsp[(1) - (1)].fnDeclStmt)->pos,(yyvsp[(1) - (1)].fnDeclStmt));
	}
    break;

  case 7:
#line 150 "parser.yacc"
    {
		(yyval.programElement)= A_ProgramFnDef((yyvsp[(1) - (1)].fnDef)->pos,(yyvsp[(1) - (1)].fnDef));
	}
    break;

  case 8:
#line 153 "parser.yacc"
    {
		(yyval.programElement)= A_ProgramNullStmt((yyvsp[(1) - (1)].pos));
	}
    break;

  case 9:
#line 157 "parser.yacc"
    {(yyval.varDeclStmt) =A_VarDeclStmt((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].varDecl)); }
    break;

  case 10:
#line 158 "parser.yacc"
    {(yyval.varDeclStmt) =A_VarDefStmt((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].varDef)); }
    break;

  case 11:
#line 160 "parser.yacc"
    {
	// let a:int;
		(yyval.varDecl) = A_VarDecl_Scalar((yyvsp[(1) - (3)].tokenId)->pos,A_VarDeclScalar((yyvsp[(1) - (3)].tokenId)->pos,(yyvsp[(1) - (3)].tokenId)->id,(yyvsp[(3) - (3)].type)));
	}
    break;

  case 12:
#line 164 "parser.yacc"
    {
		// let a;
		(yyval.varDecl) = A_VarDecl_Scalar((yyvsp[(1) - (1)].tokenId)->pos,A_VarDeclScalar((yyvsp[(1) - (1)].tokenId)->pos,(yyvsp[(1) - (1)].tokenId)->id,0));
	}
    break;

  case 13:
#line 168 "parser.yacc"
    {
		//let a[4]:int;
		(yyval.varDecl) = A_VarDecl_Array((yyvsp[(1) - (6)].tokenId)->pos,A_VarDeclArray((yyvsp[(1) - (6)].tokenId)->pos,(yyvsp[(1) - (6)].tokenId)->id,(yyvsp[(3) - (6)].tokenNum)->num,(yyvsp[(6) - (6)].type)));
	}
    break;

  case 14:
#line 172 "parser.yacc"
    {
		//let a[4];
		(yyval.varDecl) = A_VarDecl_Array((yyvsp[(1) - (4)].tokenId)->pos,A_VarDeclArray((yyvsp[(1) - (4)].tokenId)->pos,(yyvsp[(1) - (4)].tokenId)->id,(yyvsp[(3) - (4)].tokenNum)->num,0));
	}
    break;

  case 15:
#line 177 "parser.yacc"
    {
	// let a:int=1;
		(yyval.varDef) = A_VarDef_Scalar((yyvsp[(1) - (5)].tokenId)->pos,A_VarDefScalar((yyvsp[(1) - (5)].tokenId)->pos,(yyvsp[(1) - (5)].tokenId)->id,(yyvsp[(3) - (5)].type),(yyvsp[(5) - (5)].rightVal)));
	}
    break;

  case 16:
#line 181 "parser.yacc"
    {
		// let a=1;
		(yyval.varDef) = A_VarDef_Scalar((yyvsp[(1) - (3)].tokenId)->pos,A_VarDefScalar((yyvsp[(1) - (3)].tokenId)->pos,(yyvsp[(1) - (3)].tokenId)->id,0,(yyvsp[(3) - (3)].rightVal)));
	}
    break;

  case 17:
#line 185 "parser.yacc"
    {
		(yyval.varDef) = A_VarDef_Array((yyvsp[(1) - (10)].tokenId)->pos,A_VarDefArray((yyvsp[(1) - (10)].tokenId)->pos,(yyvsp[(1) - (10)].tokenId)->id,(yyvsp[(3) - (10)].tokenNum)->num,(yyvsp[(6) - (10)].type),(yyvsp[(9) - (10)].rightValList)));
	}
    break;

  case 18:
#line 188 "parser.yacc"
    {
		(yyval.varDef) = A_VarDef_Array((yyvsp[(1) - (8)].tokenId)->pos,A_VarDefArray((yyvsp[(1) - (8)].tokenId)->pos,(yyvsp[(1) - (8)].tokenId)->id,(yyvsp[(3) - (8)].tokenNum)->num,0,(yyvsp[(7) - (8)].rightValList)));
	}
    break;

  case 19:
#line 192 "parser.yacc"
    { (yyval.rightValList) = 0;}
    break;

  case 20:
#line 193 "parser.yacc"
    { 
		(yyval.rightValList) = (yyvsp[(1) - (1)].rightValList);
	}
    break;

  case 21:
#line 198 "parser.yacc"
    {
		(yyval.rightValList) = A_RightValList((yyvsp[(1) - (1)].rightVal),0);
	}
    break;

  case 22:
#line 201 "parser.yacc"
    {
		(yyval.rightValList) = A_RightValList((yyvsp[(1) - (3)].rightVal),(yyvsp[(3) - (3)].rightValList));
	}
    break;

  case 23:
#line 205 "parser.yacc"
    {
		(yyval.fnDeclStmt) = A_FnDeclStmt((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].fnDecl));
	}
    break;

  case 24:
#line 210 "parser.yacc"
    {
		(yyval.structDef) = A_StructDef((yyvsp[(1) - (5)].pos), (yyvsp[(2) - (5)].tokenId)->id, (yyvsp[(4) - (5)].paramDecl));
	}
    break;

  case 25:
#line 214 "parser.yacc"
    {
		(yyval.fnDef) = A_FnDef((yyvsp[(1) - (5)].pos),(yyvsp[(2) - (5)].fnDecl),(yyvsp[(4) - (5)].stmts));
	}
    break;

  case 26:
#line 220 "parser.yacc"
    { (yyval.stmts) = 0 ;}
    break;

  case 27:
#line 221 "parser.yacc"
    {
		(yyval.stmts) = A_CodeBlockStmtList((yyvsp[(1) - (2)].codeBlockStmt), (yyvsp[(2) - (2)].stmts));
	}
    break;

  case 28:
#line 226 "parser.yacc"
    {(yyval.leftVal)= A_IdExprLVal((yyvsp[(1) - (1)].tokenId)->pos, (yyvsp[(1) - (1)].tokenId)->id);}
    break;

  case 29:
#line 227 "parser.yacc"
    {(yyval.leftVal) = A_ArrExprLVal((yyvsp[(1) - (4)].leftVal)->pos,A_ArrayExpr((yyvsp[(1) - (4)].leftVal)->pos,(yyvsp[(1) - (4)].leftVal),(yyvsp[(3) - (4)].indexExpr)));}
    break;

  case 30:
#line 228 "parser.yacc"
    {(yyval.leftVal) =A_MemberExprLVal((yyvsp[(1) - (1)].memberExpr)->pos, (yyvsp[(1) - (1)].memberExpr));}
    break;

  case 31:
#line 231 "parser.yacc"
    {
		(yyval.assignStmt) = A_AssignStmt((yyvsp[(1) - (4)].leftVal)->pos, (yyvsp[(1) - (4)].leftVal), (yyvsp[(3) - (4)].rightVal));
	}
    break;

  case 32:
#line 237 "parser.yacc"
    {
		(yyval.whileStmt) = A_WhileStmt((yyvsp[(1) - (7)].pos),(yyvsp[(3) - (7)].boolExpr),(yyvsp[(6) - (7)].stmts));
	}
    break;

  case 33:
#line 243 "parser.yacc"
    {
		(yyval.ifStmt) = A_IfStmt((yyvsp[(1) - (7)].pos),(yyvsp[(3) - (7)].boolExpr),(yyvsp[(6) - (7)].stmts),0);
	}
    break;

  case 34:
#line 248 "parser.yacc"
    {
		(yyval.ifStmt) = A_IfStmt((yyvsp[(1) - (11)].pos),(yyvsp[(3) - (11)].boolExpr),(yyvsp[(6) - (11)].stmts),(yyvsp[(10) - (11)].stmts));
	}
    break;

  case 35:
#line 253 "parser.yacc"
    { (yyval.codeBlockStmt) = A_BlockNullStmt((yyvsp[(1) - (1)].pos));}
    break;

  case 36:
#line 254 "parser.yacc"
    { (yyval.codeBlockStmt) = A_BlockVarDeclStmt((yyvsp[(1) - (1)].varDeclStmt)->pos, (yyvsp[(1) - (1)].varDeclStmt));}
    break;

  case 37:
#line 255 "parser.yacc"
    { (yyval.codeBlockStmt) = A_BlockAssignStmt((yyvsp[(1) - (1)].assignStmt)->pos, (yyvsp[(1) - (1)].assignStmt));}
    break;

  case 38:
#line 256 "parser.yacc"
    { (yyval.codeBlockStmt) = A_BlockCallStmt((yyvsp[(1) - (2)].fnCall)->pos, A_CallStmt((yyvsp[(1) - (2)].fnCall)->pos, (yyvsp[(1) - (2)].fnCall)));}
    break;

  case 39:
#line 257 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockIfStmt((yyvsp[(1) - (1)].ifStmt)->pos,(yyvsp[(1) - (1)].ifStmt));}
    break;

  case 40:
#line 258 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockWhileStmt((yyvsp[(1) - (1)].whileStmt)->pos,(yyvsp[(1) - (1)].whileStmt));}
    break;

  case 41:
#line 259 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockReturnStmt((yyvsp[(1) - (3)].pos),A_ReturnStmt((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].rightVal)));}
    break;

  case 42:
#line 260 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockReturnStmt((yyvsp[(1) - (2)].pos),A_ReturnStmt((yyvsp[(1) - (2)].pos),0));}
    break;

  case 43:
#line 261 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockContinueStmt((yyvsp[(1) - (2)].pos));}
    break;

  case 44:
#line 262 "parser.yacc"
    {(yyval.codeBlockStmt) = A_BlockBreakStmt((yyvsp[(1) - (2)].pos));}
    break;

  case 45:
#line 265 "parser.yacc"
    {(yyval.boolExpr)=(yyvsp[(1) - (1)].boolExpr);}
    break;

  case 46:
#line 267 "parser.yacc"
    {
		(yyval.fnDecl) = A_FnDecl((yyvsp[(1) - (5)].tokenId)->pos,(yyvsp[(1) - (5)].tokenId)->id,A_ParamDecl((yyvsp[(3) - (5)].paramDecl)),(yyvsp[(5) - (5)].type));
	}
    break;

  case 47:
#line 270 "parser.yacc"
    {
		(yyval.fnDecl) = A_FnDecl((yyvsp[(1) - (4)].tokenId)->pos,(yyvsp[(1) - (4)].tokenId)->id,A_ParamDecl((yyvsp[(3) - (4)].paramDecl)),0);
	}
    break;

  case 48:
#line 274 "parser.yacc"
    {	(yyval.paramDecl) = 0;}
    break;

  case 49:
#line 275 "parser.yacc"
    {
		(yyval.paramDecl)=(yyvsp[(1) - (1)].paramDecl);
	}
    break;

  case 50:
#line 280 "parser.yacc"
    {
		(yyval.paramDecl)=A_VarDeclList((yyvsp[(1) - (1)].varDecl),0);
	}
    break;

  case 51:
#line 283 "parser.yacc"
    {
		(yyval.paramDecl)=A_VarDeclList((yyvsp[(1) - (3)].varDecl),(yyvsp[(3) - (3)].paramDecl));
	}
    break;

  case 52:
#line 287 "parser.yacc"
    {
		(yyval.type) = (yyvsp[(2) - (2)].type);
	}
    break;

  case 53:
#line 292 "parser.yacc"
    {
		(yyval.type) = A_StructType((yyvsp[(1) - (1)].tokenId)->pos, (yyvsp[(1) - (1)].tokenId)->id);
	}
    break;

  case 54:
#line 295 "parser.yacc"
    {
		(yyval.type) = (yyvsp[(1) - (1)].type);
	}
    break;

  case 55:
#line 300 "parser.yacc"
    {(yyval.rightVal)=A_ArithExprRVal((yyvsp[(1) - (1)].arithExpr)->pos, (yyvsp[(1) - (1)].arithExpr));}
    break;

  case 56:
#line 301 "parser.yacc"
    {(yyval.rightVal)=A_BoolExprRVal((yyvsp[(1) - (1)].boolExpr)->pos, (yyvsp[(1) - (1)].boolExpr));}
    break;

  case 57:
#line 303 "parser.yacc"
    {(yyval.arithExpr)=A_ArithBiOp_Expr((yyvsp[(1) - (1)].arithBiOpExpr)->pos,(yyvsp[(1) - (1)].arithBiOpExpr));}
    break;

  case 58:
#line 304 "parser.yacc"
    {(yyval.arithExpr)=A_ExprUnit((yyvsp[(1) - (1)].exprUnit)->pos,(yyvsp[(1) - (1)].exprUnit));}
    break;

  case 59:
#line 306 "parser.yacc"
    {(yyval.arithBiOpExpr)=A_ArithBiOpExpr((yyvsp[(1) - (3)].arithExpr)->pos,(yyvsp[(2) - (3)].op),(yyvsp[(1) - (3)].arithExpr),(yyvsp[(3) - (3)].arithExpr));}
    break;

  case 60:
#line 307 "parser.yacc"
    {(yyval.arithBiOpExpr)=A_ArithBiOpExpr((yyvsp[(1) - (3)].arithExpr)->pos,(yyvsp[(2) - (3)].op),(yyvsp[(1) - (3)].arithExpr),(yyvsp[(3) - (3)].arithExpr));}
    break;

  case 61:
#line 308 "parser.yacc"
    {(yyval.arithBiOpExpr)=A_ArithBiOpExpr((yyvsp[(1) - (3)].arithExpr)->pos,(yyvsp[(2) - (3)].op),(yyvsp[(1) - (3)].arithExpr),(yyvsp[(3) - (3)].arithExpr));}
    break;

  case 62:
#line 309 "parser.yacc"
    {(yyval.arithBiOpExpr)=A_ArithBiOpExpr((yyvsp[(1) - (3)].arithExpr)->pos,(yyvsp[(2) - (3)].op),(yyvsp[(1) - (3)].arithExpr),(yyvsp[(3) - (3)].arithExpr));}
    break;

  case 63:
#line 311 "parser.yacc"
    {(yyval.exprUnit)=A_NumExprUnit((yyvsp[(1) - (1)].tokenNum)->pos, (yyvsp[(1) - (1)].tokenNum)->num);}
    break;

  case 64:
#line 312 "parser.yacc"
    {(yyval.exprUnit)= A_IdExprUnit((yyvsp[(1) - (1)].tokenId)->pos, (yyvsp[(1) - (1)].tokenId)->id);}
    break;

  case 65:
#line 313 "parser.yacc"
    {(yyval.exprUnit) = A_ArithExprUnit((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].arithExpr));}
    break;

  case 66:
#line 314 "parser.yacc"
    {(yyval.exprUnit)=A_CallExprUnit((yyvsp[(1) - (1)].fnCall)->pos,(yyvsp[(1) - (1)].fnCall));}
    break;

  case 67:
#line 315 "parser.yacc"
    {(yyval.exprUnit) = A_ArrayExprUnit((yyvsp[(1) - (4)].leftVal)->pos,A_ArrayExpr((yyvsp[(1) - (4)].leftVal)->pos,(yyvsp[(1) - (4)].leftVal),(yyvsp[(3) - (4)].indexExpr)));}
    break;

  case 68:
#line 316 "parser.yacc"
    {(yyval.exprUnit) =A_MemberExprUnit((yyvsp[(1) - (1)].memberExpr)->pos, (yyvsp[(1) - (1)].memberExpr));}
    break;

  case 69:
#line 317 "parser.yacc"
    {(yyval.exprUnit)=A_ArithUExprUnit((yyvsp[(1) - (1)].arithUExpr)->pos, (yyvsp[(1) - (1)].arithUExpr));}
    break;

  case 70:
#line 319 "parser.yacc"
    {(yyval.fnCall) = A_FnCall((yyvsp[(1) - (4)].tokenId)->pos,(yyvsp[(1) - (4)].tokenId)->id,(yyvsp[(3) - (4)].rightValList));}
    break;

  case 71:
#line 321 "parser.yacc"
    {(yyval.arithUExpr)=A_ArithUExpr((yyvsp[(2) - (2)].exprUnit)->pos,A_neg,(yyvsp[(2) - (2)].exprUnit));}
    break;

  case 72:
#line 324 "parser.yacc"
    {(yyval.memberExpr)=A_MemberExpr((yyvsp[(1) - (3)].leftVal)->pos,(yyvsp[(1) - (3)].leftVal),(yyvsp[(3) - (3)].tokenId)->id );}
    break;

  case 73:
#line 326 "parser.yacc"
    {(yyval.indexExpr)=A_NumIndexExpr((yyvsp[(1) - (1)].tokenNum)->pos,(yyvsp[(1) - (1)].tokenNum)->num);}
    break;

  case 74:
#line 327 "parser.yacc"
    {(yyval.indexExpr)=A_IdIndexExpr((yyvsp[(1) - (1)].tokenId)->pos,(yyvsp[(1) - (1)].tokenId)->id);}
    break;

  case 75:
#line 329 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_lt,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 76:
#line 330 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_le,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 77:
#line 331 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_gt,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 78:
#line 332 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_ge,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 79:
#line 333 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_eq,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 80:
#line 334 "parser.yacc"
    {(yyval.comExpr)=A_ComExpr((yyvsp[(1) - (3)].exprUnit)->pos,A_ne,(yyvsp[(1) - (3)].exprUnit),(yyvsp[(3) - (3)].exprUnit));}
    break;

  case 81:
#line 336 "parser.yacc"
    {(yyval.boolExpr)=A_BoolBiOp_Expr((yyvsp[(1) - (1)].boolBiOpExpr)->pos,(yyvsp[(1) - (1)].boolBiOpExpr));}
    break;

  case 82:
#line 337 "parser.yacc"
    {(yyval.boolExpr)=A_BoolExpr((yyvsp[(1) - (1)].boolUnit)->pos,(yyvsp[(1) - (1)].boolUnit));}
    break;

  case 83:
#line 339 "parser.yacc"
    {(yyval.boolBiOpExpr)=A_BoolBiOpExpr((yyvsp[(1) - (3)].boolExpr)->pos,A_and,(yyvsp[(1) - (3)].boolExpr),(yyvsp[(3) - (3)].boolExpr));}
    break;

  case 84:
#line 340 "parser.yacc"
    {(yyval.boolBiOpExpr)=A_BoolBiOpExpr((yyvsp[(1) - (3)].boolExpr)->pos,A_or,(yyvsp[(1) - (3)].boolExpr),(yyvsp[(3) - (3)].boolExpr));}
    break;

  case 85:
#line 342 "parser.yacc"
    {(yyval.boolUnit) = A_ComExprUnit((yyvsp[(1) - (1)].comExpr)->pos, (yyvsp[(1) - (1)].comExpr));}
    break;

  case 86:
#line 343 "parser.yacc"
    {(yyval.boolUnit)=A_BoolExprUnit((yyvsp[(1) - (3)].pos),(yyvsp[(2) - (3)].boolExpr));}
    break;

  case 87:
#line 344 "parser.yacc"
    {(yyval.boolUnit)=A_BoolUOpExprUnit((yyvsp[(1) - (1)].boolUOpExpr)->pos,(yyvsp[(1) - (1)].boolUOpExpr));}
    break;

  case 88:
#line 346 "parser.yacc"
    {(yyval.boolUOpExpr)=A_BoolUOpExpr((yyvsp[(1) - (2)].pos),A_not,(yyvsp[(2) - (2)].boolUnit));}
    break;


/* Line 1267 of yacc.c.  */
#line 2125 "y.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 350 "parser.yacc"

A_pos my_pos()
{
	return A_Pos(line,col);
}
extern "C"{
void yyerror(char * s)
{
  fprintf(stderr, "%s\n",s);
}
int yywrap()
{
  return(1);
}

}



