/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "miniL.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <string>
#include <stack>
#include <vector>
#include <map>

using namespace std;

extern FILE * yyin;
extern int yylex();
extern int currLine;
extern int currPos; 

enum SYMBOL_TYPE {INT, ARR, FUNC};

string getSymbolTypeName(SYMBOL_TYPE type)
{
  if(type == INT)
  {
    return "INT";
  }
  else if (type == ARR)
  {
    return "ARR";
  }
  else if (type == FUNC)
  {
    return "FUNC";
  }
  else
  {
    return "ERROR";
  }
}

void yyerror(const char * errorMessage)
{
	printf("Error: On line %d, column %d: %s\n", currLine, currPos, errorMessage);
}

struct Symbol 
{
  string name;
  SYMBOL_TYPE type; 
  
  int value;
  int size;

  Symbol(): value(0), size(0), name(), type() {};  
};

struct Function 
{
  string name;
  vector<Symbol> declarations;
};

vector<Function> symbol_table;
bool mainExists = 0;
bool mainDuplicateExists = 0;


Function *get_function() {
  int last = symbol_table.size()-1;
  return &symbol_table[last];
}

bool findSymbol(std::string &value) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value) {
      return true;
    }
  }
  return false;
}

bool findFunction(std::string &value) {
  for(int i=0; i<symbol_table.size(); i++) {
     Function *f = &symbol_table[i];
    if (f->name == value) {
      return true;
    }
  }
  return false;
}

bool checkSymbolExists(std::string &value) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value) {
      return true;
    }
  }

  string error = "Symbol \"" + value + "\" is not declared.";
  yyerror(error.c_str());
  return false;
}

bool checkSymbolIsOfType(std::string &value, SYMBOL_TYPE assertType) {
  Function *f = get_function();
  for(int i=0; i < f->declarations.size(); i++) {
    Symbol *s = &f->declarations[i];
    if (s->name == value)
    {
      if(s->type == assertType)
      {
        return true;
      }
      else
      {
        string error = "Symbol \"" + value + "\" is of type \"" + getSymbolTypeName(s->type) + ".";
        yyerror(error.c_str());
        return false;
      }
      
    }
  }


  return false;
}

bool checkFunctionExists(std::string &value) {
  for(int i=0; i<symbol_table.size(); i++) {
     Function *f = &symbol_table[i];
    if (f->name == value) {
      return true;
    }
  }
  
  string error = "Function \"" + value + "\" is not declared.";
  yyerror(error.c_str());
  return false;
}

void add_function_to_symbol_table(std::string &value) {

  if(findFunction(value))
  {
    string error = "Function \"" + value + "\" is already declared.";
    yyerror(error.c_str());
  }
  else
  {
    Function f; 
    f.name = value; 
    symbol_table.push_back(f);
    if(value == "main")
    {
      if(mainExists == 1)
      {
        mainDuplicateExists = 1;
      }
      mainExists = 1;
    }
  }
}


void add_variable_to_symbol_table(std::string &name, int value, int size, SYMBOL_TYPE t) {
  
  if(findSymbol(name))
  {
    string error = "Symbol \"" + name + "\" is already declared.";
    yyerror(error.c_str());
  }
  else
  {
    Symbol s;
    s.name = name;
    s.type = t;
    s.value = value;
    s.size = size;
    Function *f = get_function();
    f->declarations.push_back(s);
  }
}

void print_symbol_table(void) {
  printf("symbol table:\n");
  printf("--------------------\n");
  for(int i=0; i<symbol_table.size(); i++) {
    printf("function: %s\n", symbol_table[i].name.c_str());
    for(int j=0; j<symbol_table[i].declarations.size(); j++) {
      printf("  locals: %s\n", symbol_table[i].declarations[j].name.c_str());
    }
  }
  printf("--------------------\n");
}

int tempCount = 0;
int labelCount = 0;
int paramCount = 0;

string makeTemp() {
  stringstream ss;
  ss << tempCount++;
  string temp = "__temp" + ss.str();
  return temp;
}

string makeLabel() {
  stringstream ss;
  ss << labelCount++;
  string temp = "__label" + ss.str();
  return temp;
}

stack<string> varStack; 
stack<string> identStack;
stack<string> expStack; 
vector<string> label_vector;
stack<string> paramStack;
stringstream inCode;
ostringstream outCode;

stack<string> ands;
stack<string> ors;



#line 299 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FUNCTION = 258,
    BEGIN_PARAMS = 259,
    END_PARAMS = 260,
    BEGIN_LOCALS = 261,
    END_LOCALS = 262,
    BEGIN_BODY = 263,
    END_BODY = 264,
    INTEGER = 265,
    ARRAY = 266,
    ENUM = 267,
    OF = 268,
    IF = 269,
    THEN = 270,
    ENDIF = 271,
    ELSE = 272,
    FOR = 273,
    WHILE = 274,
    DO = 275,
    BEGINLOOP = 276,
    ENDLOOP = 277,
    CONTINUE = 278,
    READ = 279,
    WRITE = 280,
    TRUE = 281,
    FALSE = 282,
    RETURN = 283,
    SEMICOLON = 284,
    COLON = 285,
    COMMA = 286,
    ASSIGN = 287,
    OR = 288,
    AND = 289,
    NOT = 290,
    NEQ = 291,
    EQ = 292,
    GTE = 293,
    GT = 294,
    LTE = 295,
    LT = 296,
    ADD = 297,
    SUB = 298,
    MULT = 299,
    DIV = 300,
    MOD = 301,
    UMINUS = 302,
    L_SQUARE_BRACKET = 303,
    R_SQUARE_BRACKET = 304,
    L_PAREN = 305,
    R_PAREN = 306,
    IDENT = 307,
    NUMBER = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 236 "miniL.y" /* yacc.c:355  */

  /* put your types here */
  char* identifierVal;
  int numberVal;
  struct attributesbison 
  {
    char name[100];
    char ind[100];
    int type;
    int value;
    int size;
  } attr;

#line 407 "y.tab.c" /* yacc.c:355  */
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


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 438 "y.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   163

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  158

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   294,   294,   297,   300,   304,   307,   312,   318,   311,
     337,   341,   344,   347,   351,   355,   358,   370,   386,   392,
     398,   406,   409,   412,   415,   418,   421,   424,   427,   431,
     448,   447,   466,   475,   465,   493,   492,   525,   524,   544,
     565,   567,   571,   597,   613,   624,   639,   645,   648,   663,
     669,   672,   687,   695,   703,   711,   717,   721,   724,   727,
     730,   733,   736,   739,   743,   749,   757,   766,   772,   780,
     788,   797,   815,   857,   865,   886,   891,   907,   922,   933,
     938,   944,   968,   980,   992
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FUNCTION", "BEGIN_PARAMS", "END_PARAMS",
  "BEGIN_LOCALS", "END_LOCALS", "BEGIN_BODY", "END_BODY", "INTEGER",
  "ARRAY", "ENUM", "OF", "IF", "THEN", "ENDIF", "ELSE", "FOR", "WHILE",
  "DO", "BEGINLOOP", "ENDLOOP", "CONTINUE", "READ", "WRITE", "TRUE",
  "FALSE", "RETURN", "SEMICOLON", "COLON", "COMMA", "ASSIGN", "OR", "AND",
  "NOT", "NEQ", "EQ", "GTE", "GT", "LTE", "LT", "ADD", "SUB", "MULT",
  "DIV", "MOD", "UMINUS", "L_SQUARE_BRACKET", "R_SQUARE_BRACKET",
  "L_PAREN", "R_PAREN", "IDENT", "NUMBER", "$accept", "program",
  "functions", "functions_spec", "function", "$@1", "$@2", "declarations",
  "declarations_spec", "statements", "statementsEnding", "declaration",
  "identifiers", "statement", "svar", "sif", "$@3", "$@4", "$@5", "swhile",
  "$@6", "sdo", "$@7", "sread", "vars", "swrite", "scontinue", "sreturn",
  "bool_expr", "bool_exprs", "relation_and_expr", "relation_and_exprs",
  "relation_expr", "relation_expr_ending", "nots", "comp", "expression",
  "mul_exp", "term", "expressions", "var", "ident", "number", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308
};
# endif

#define YYPACT_NINF -74

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-74)))

#define YYTABLE_NINF -6

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      75,   -74,   -31,    14,   -74,    20,   -74,   -74,   -74,   -74,
     -74,    -1,    26,   -31,   -74,     5,    10,    48,    37,   -31,
      89,   -31,    42,   -74,   -74,   -74,     3,    47,   -74,   -31,
       8,   -31,    95,   -74,    58,    60,   106,   118,   -74,    70,
     128,   104,   104,   119,   -74,   -31,   -31,    65,   132,   113,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   111,    96,
     -74,   -74,   130,   114,   112,   -17,   127,   -74,   120,   120,
      30,    65,    44,    88,   -74,   -74,   -32,   -74,   -74,    70,
      65,    65,   -74,   104,   -74,   104,   -74,   -74,   -74,    17,
     -74,    87,   -74,    70,   -31,   -74,   -74,    65,   -74,   -74,
      11,    65,    65,    65,    65,    65,    53,   -74,   -74,    44,
      22,    70,    70,   114,   112,    98,   -74,   -74,   -74,   -74,
     -74,   -74,    65,    70,   131,   120,    15,   -74,    88,    88,
     -74,   -74,   -74,   -74,    44,   -27,   -74,   134,   137,   -74,
     -74,   -74,    44,   133,   138,   -74,   -74,    65,   -74,   -74,
     -74,   -74,   104,    44,    70,   -74,   140,   -74
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     3,     0,     0,     2,     5,    83,     7,     1,     6,
       4,     0,     0,    11,     8,     0,     0,    20,     0,    11,
       0,     0,     0,    12,    10,    16,     0,     0,    19,    11,
       0,     0,     0,    84,     0,     0,     0,     0,    18,     0,
       0,    57,    57,     0,    43,     0,     0,     0,     0,     0,
      21,    22,    23,    24,    25,    26,    27,    28,     0,    82,
      17,    56,     0,    47,    50,     0,     0,    37,    40,    40,
       0,     0,    44,    64,    67,    71,    82,    73,     9,    15,
       0,     0,    30,    57,    45,    57,    48,    53,    54,    57,
      51,     0,    35,     0,     0,    39,    42,     0,    72,    74,
       0,     0,     0,     0,     0,     0,     0,    14,    13,    29,
       0,     0,     0,    47,    50,     0,    59,    58,    63,    61,
      62,    60,     0,     0,     0,    40,     0,    75,    65,    66,
      68,    69,    70,    78,    80,     0,    81,     0,     0,    46,
      49,    55,    52,     0,     0,    41,    76,     0,    77,    31,
      33,    36,    57,    79,     0,    38,     0,    34
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -74,   -74,   153,   -74,   -74,   -74,   -74,    -7,   -74,   -73,
     -74,   -74,    -6,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -66,   -74,   -74,   -74,   -40,    46,
      77,    49,    76,   -74,   -74,   -74,   -34,    18,    32,   -74,
     -38,    -2,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    10,     5,    11,    18,    14,    24,    48,
     108,    15,    16,    49,    50,    51,   111,   112,   154,    52,
     123,    53,    93,    54,    95,    55,    56,    57,    62,    84,
      63,    86,    64,    90,    65,   122,   100,    73,    74,   135,
      75,    76,    77
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       7,    58,    66,    96,   147,    34,   107,    68,    69,    87,
      88,    17,    23,    72,     8,    28,    81,    17,   106,    17,
     124,     6,    32,     2,   148,    35,    70,    17,    12,    17,
      13,    91,    98,    89,    19,     6,    33,    59,   137,   138,
      20,    58,    22,    59,    59,    99,   109,   110,    29,   115,
     143,    30,    61,   101,   102,    58,   125,   101,   102,   145,
      70,    33,   127,   126,   101,   102,   146,    71,    59,     6,
      33,   136,   134,    58,    58,    -5,     1,    59,     2,    21,
      97,   156,     6,    33,    41,    58,   101,   102,   142,    42,
      43,    59,    59,    44,    45,    46,    70,    31,    47,    25,
      26,    27,    36,    71,   133,     6,    33,    37,    70,    59,
      59,    38,   155,   153,    39,    71,    58,     6,    33,   128,
     129,    59,     6,   116,   117,   118,   119,   120,   121,   101,
     102,    40,   103,   104,   105,   130,   131,   132,    60,    61,
      67,    78,    79,    80,    81,    82,    85,    83,    92,   141,
     149,    94,    59,   144,   150,   151,   157,   152,     9,   139,
     113,   114,     0,   140
};

static const yytype_int16 yycheck[] =
{
       2,    39,    42,    69,    31,    30,    79,    45,    46,    26,
      27,    13,    19,    47,     0,    21,    48,    19,    50,    21,
      93,    52,    29,     3,    51,    31,    43,    29,    29,    31,
       4,    65,    70,    50,    29,    52,    53,    39,   111,   112,
      30,    79,     5,    45,    46,    70,    80,    81,     6,    89,
     123,    48,    35,    42,    43,    93,    94,    42,    43,   125,
      43,    53,    51,    97,    42,    43,    51,    50,    70,    52,
      53,    49,   106,   111,   112,     0,     1,    79,     3,    31,
      50,   154,    52,    53,    14,   123,    42,    43,   122,    19,
      20,    93,    94,    23,    24,    25,    43,    50,    28,    10,
      11,    12,     7,    50,    51,    52,    53,    49,    43,   111,
     112,    51,   152,   147,     8,    50,   154,    52,    53,   101,
     102,   123,    52,    36,    37,    38,    39,    40,    41,    42,
      43,    13,    44,    45,    46,   103,   104,   105,    10,    35,
      21,     9,    29,    32,    48,    15,    34,    33,    21,    51,
      16,    31,   154,    22,    17,    22,    16,    19,     5,   113,
      83,    85,    -1,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    55,    56,    58,    52,    95,     0,    56,
      57,    59,    29,     4,    61,    65,    66,    95,    60,    29,
      30,    31,     5,    61,    62,    10,    11,    12,    66,     6,
      48,    50,    61,    53,    96,    66,     7,    49,    51,     8,
      13,    14,    19,    20,    23,    24,    25,    28,    63,    67,
      68,    69,    73,    75,    77,    79,    80,    81,    94,    95,
      10,    35,    82,    84,    86,    88,    82,    21,    94,    94,
      43,    50,    90,    91,    92,    94,    95,    96,     9,    29,
      32,    48,    15,    33,    83,    34,    85,    26,    27,    50,
      87,    90,    21,    76,    31,    78,    78,    50,    94,    96,
      90,    42,    43,    44,    45,    46,    50,    63,    64,    90,
      90,    70,    71,    84,    86,    82,    36,    37,    38,    39,
      40,    41,    89,    74,    63,    94,    90,    51,    91,    91,
      92,    92,    92,    51,    90,    93,    49,    63,    63,    83,
      85,    51,    90,    63,    22,    78,    51,    31,    51,    16,
      17,    22,    19,    90,    72,    82,    63,    16
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    55,    56,    56,    57,    59,    60,    58,
      61,    61,    62,    63,    64,    64,    65,    65,    65,    66,
      66,    67,    67,    67,    67,    67,    67,    67,    67,    68,
      70,    69,    71,    72,    69,    74,    73,    76,    75,    77,
      78,    78,    79,    80,    81,    82,    83,    83,    84,    85,
      85,    86,    87,    87,    87,    87,    88,    88,    89,    89,
      89,    89,    89,    89,    90,    90,    90,    91,    91,    91,
      91,    92,    92,    92,    92,    92,    92,    92,    92,    93,
      93,    94,    94,    95,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     1,     0,     0,    14,
       3,     0,     1,     3,     1,     0,     3,     8,     6,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     6,     0,     0,     9,     0,     6,     0,     7,     3,
       0,     3,     3,     1,     2,     2,     3,     0,     2,     3,
       0,     2,     3,     1,     1,     3,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     1,     2,     3,     4,     4,     3,     3,
       1,     4,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
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

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 295 "miniL.y" /* yacc.c:1646  */
    {printf("program->functions\n");}
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 297 "miniL.y" /* yacc.c:1646  */
    {yyerrok; yyclearin;}
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 301 "miniL.y" /* yacc.c:1646  */
    {printf("functions->function functions_spec\n");}
#line 1742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 304 "miniL.y" /* yacc.c:1646  */
    {printf("functions->epsilon\n");}
#line 1748 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 308 "miniL.y" /* yacc.c:1646  */
    {printf("functions_spec->functions\n");}
#line 1754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 312 "miniL.y" /* yacc.c:1646  */
    {
                            inCode << "func " << (yyvsp[0].attr).name << endl;
                            string name = string((yyvsp[0].attr).name);
                            add_function_to_symbol_table(name);
                          }
#line 1764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 318 "miniL.y" /* yacc.c:1646  */
    {
                            paramCount = 0;
                            while (!paramStack.empty())
                            {
                              inCode << "= " << paramStack.top() << ", " << "$" << paramCount++ << endl;
                              paramStack.pop();
                            }
                          }
#line 1777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 327 "miniL.y" /* yacc.c:1646  */
    {
                            outCode << "endfunc\n";
                        
                            while (!paramStack.empty()) 
                            {
                              paramStack.pop();
                            }
                          }
#line 1790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 338 "miniL.y" /* yacc.c:1646  */
    {printf("declarations->declaration SEMICOLON declarations_spec\n");}
#line 1796 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 341 "miniL.y" /* yacc.c:1646  */
    {printf("declarations->epsilon\n");}
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 345 "miniL.y" /* yacc.c:1646  */
    {printf("declarations_spec->declarations\n");}
#line 1808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 348 "miniL.y" /* yacc.c:1646  */
    {printf("statements->statement SEMICOLON statementsEnding\n");}
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 352 "miniL.y" /* yacc.c:1646  */
    {printf("statementsEnding->statements\n");}
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 355 "miniL.y" /* yacc.c:1646  */
    {printf("statementsEnding->epsilon\n");}
#line 1826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 359 "miniL.y" /* yacc.c:1646  */
    {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = INT;
                              add_variable_to_symbol_table(name, 0, 0, type);
                              inCode << ". " << name << endl;
                              identStack.pop(); 
                            }
                          }
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 371 "miniL.y" /* yacc.c:1646  */
    {
                            while(!identStack.empty())
                            {
                              string name = identStack.top();
                              SYMBOL_TYPE type = ARR;
                              add_variable_to_symbol_table(name, 0, (yyvsp[-3].attr).value, type);
                              if((yyvsp[-3].attr).value <= 0)
                              {
                                yyerror("Error: Size of array should be a positive integer.");
                              }
                              inCode << ".[] " << name << ", " << (yyvsp[-3].attr).value << endl;
                              identStack.pop(); 
                            } 
                          }
#line 1860 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 387 "miniL.y" /* yacc.c:1646  */
    {
                            //todo FOR ENUMS!! printf("declaration->identifiers COLON ENUM L_PAREN identifiers R_PAREN\n");
                          }
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 393 "miniL.y" /* yacc.c:1646  */
    {
                            identStack.push(string((yyvsp[-2].attr).name));
                            paramStack.push(string((yyvsp[-2].attr).name));
                            printf("identifiers->ident COMMA identifiers\n");}
#line 1877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 399 "miniL.y" /* yacc.c:1646  */
    {
                            identStack.push(string((yyvsp[0].attr).name));
                            paramStack.push(string((yyvsp[0].attr).name));
                            printf("identifiers->ident\n");
                            }
#line 1887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 407 "miniL.y" /* yacc.c:1646  */
    {printf("statement->svar\n");}
#line 1893 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 410 "miniL.y" /* yacc.c:1646  */
    {printf("statement->sif\n");}
#line 1899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 413 "miniL.y" /* yacc.c:1646  */
    {printf("statement->swhile\n");}
#line 1905 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 416 "miniL.y" /* yacc.c:1646  */
    {printf("statement->sdo\n");}
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 419 "miniL.y" /* yacc.c:1646  */
    {printf("statement->sread\n");}
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 422 "miniL.y" /* yacc.c:1646  */
    {printf("statement->swrite\n");}
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 425 "miniL.y" /* yacc.c:1646  */
    {printf("statement->scontinue\n");}
#line 1929 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 428 "miniL.y" /* yacc.c:1646  */
    {printf("statement->sreturn\n");}
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 432 "miniL.y" /* yacc.c:1646  */
    {
                            if((yyvsp[-2].attr).type == ARR)
                            {
                              inCode << "[]= " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[-2].attr).ind) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            }
                            else 
                            {
                              inCode << "= " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            }
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");	
                          }
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 448 "miniL.y" /* yacc.c:1646  */
    {
                            string start = makeLabel();
                            string endif = makeLabel();
                            label_vector.push_back(endif); 
                            inCode << "?:= " << start << ", " << const_cast<char*>((yyvsp[-1].attr).name) << endl;
                            inCode << ":= " << endif << endl;
                            inCode << ": " << start << endl;
                          }
#line 1966 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 457 "miniL.y" /* yacc.c:1646  */
    {
                            inCode << ": " << label_vector.back() << endl;
                            label_vector.pop_back();             
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                          }
#line 1978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 466 "miniL.y" /* yacc.c:1646  */
    {
                            string start = makeLabel();
                            string endif = makeLabel();
                            label_vector.push_back(endif); 
                            inCode << "?:= " << start << ", " << const_cast<char*>((yyvsp[-1].attr).name) << endl;
                            inCode << ":= " << endif << endl;
                            inCode << ": " << start << endl;
                          }
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 475 "miniL.y" /* yacc.c:1646  */
    {
                            string label = makeLabel(); 
                            inCode << ":= " << label << endl;
                            inCode << ": " << label_vector.back() << endl;
                            label_vector.pop_back();
                            label_vector.push_back(label);
                          }
#line 2003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 483 "miniL.y" /* yacc.c:1646  */
    {
                            inCode << ": " << label_vector.back() << endl;
                            label_vector.pop_back();             
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                          }
#line 2015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 493 "miniL.y" /* yacc.c:1646  */
    {
                            string condition = makeLabel();
                            string endlabel = makeLabel();
                            string start = makeLabel();
                            outCode << ": " << start << endl;
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                            inCode << "?:= " << condition << ", " << const_cast<char*>((yyvsp[-1].attr).name) << endl;
                            inCode << ":= " << endlabel << endl;
                            inCode << ": " << condition << endl;
                            label_vector.push_back(start);
                            label_vector.push_back(endlabel);                            
                          }
#line 2034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 508 "miniL.y" /* yacc.c:1646  */
    {
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                            string endlabel = label_vector.back();
                            label_vector.pop_back();
                            string start = label_vector.back();
                            label_vector.pop_back();
                            inCode << ":= " << start << endl;
                            inCode << ": " << endlabel << endl;
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                          }
#line 2053 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 525 "miniL.y" /* yacc.c:1646  */
    {
                            string start = makeLabel();
                            label_vector.push_back(start);
                            outCode << ": " << start << endl;
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");
                          }
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 534 "miniL.y" /* yacc.c:1646  */
    {
                            string start = label_vector.back();
                            inCode << "?:= " << start << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            label_vector.pop_back(); 
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");                            
                          }
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 545 "miniL.y" /* yacc.c:1646  */
    {
                            varStack.push((yyvsp[-1].attr).name);
                            while (!varStack.empty()) 
                            {
                                if ((yyvsp[-1].attr).type == 0) {
                                    inCode << ".< " << varStack.top() << endl;
                                    varStack.pop();
                                }
                                else {
                                    inCode << ".[]< " << varStack.top() << ", "  <<  const_cast<char*>((yyvsp[-1].attr).ind) << endl;
                                    varStack.pop();
                                }
                            }
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");                            
                          }
#line 2101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 565 "miniL.y" /* yacc.c:1646  */
    {}
#line 2107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 568 "miniL.y" /* yacc.c:1646  */
    {varStack.push((yyvsp[-1].attr).name);}
#line 2113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 572 "miniL.y" /* yacc.c:1646  */
    {
                            varStack.push((yyvsp[-1].attr).name);
                            while (!varStack.empty()) 
                            {
                              if ((yyvsp[-1].attr).type == 0) 
                              {
                                inCode << ".> " << varStack.top() << endl;
                                varStack.pop();
                              }
                              else 
                              {
                                string out = makeTemp();
                                strcpy((yyval.attr).name, out.c_str());
                                inCode << ". " << out << endl;
                                inCode << "=[] " << out << ", " << const_cast<char*>((yyvsp[-1].attr).name) << ", " << const_cast<char*>((yyvsp[-1].attr).ind) << endl;
                                inCode << ".[]> " << varStack.top() << ", "  <<  const_cast<char*>((yyvsp[-1].attr).ind) << endl;
                                varStack.pop();
                              }
                            }
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");                            
                          }
#line 2141 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 598 "miniL.y" /* yacc.c:1646  */
    {
                            if (!label_vector.empty())
                            {
                              inCode << ":= " << label_vector.back() << endl;
                              outCode << inCode.rdbuf();
                              inCode.clear();
                              inCode.str(" ");
                            }
                            else 
                            {
                              yyerror("Error: Continue used outside of loop");
                            }
                          }
#line 2159 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 614 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].attr).value;
                            strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                            inCode << "ret " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            outCode << inCode.rdbuf();
                            inCode.clear();
                            inCode.str(" ");                            
                          }
#line 2172 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 625 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            strcpy((yyval.attr).name, out.c_str());
                            inCode << ". " << out << endl;
                            inCode << "= " << out << ", " << (yyvsp[-1].attr).name << endl;;

                            while(!ors.empty())
                            {
                              string name = ors.top();
                              inCode << "|| " << out << ", " << out << ", " << name << endl; 
                            }
                          }
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 640 "miniL.y" /* yacc.c:1646  */
    {
                            ors.push(string((yyvsp[-1].attr).name));
                          }
#line 2197 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 645 "miniL.y" /* yacc.c:1646  */
    {printf("bool_exprs->epsilon\n");}
#line 2203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 649 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            strcpy((yyval.attr).name, out.c_str());
                            inCode << ". " << out << endl;
                            inCode << "= " << out << ", " << (yyvsp[-1].attr).name << endl;;

                            while(!ands.empty())
                            {
                              string name = ands.top();
                              inCode << "&& " << out << ", " << out << ", " << name << endl; 
                            }
                          }
#line 2220 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 664 "miniL.y" /* yacc.c:1646  */
    {
                            ands.push(string((yyvsp[-1].attr).name));
                          }
#line 2228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 669 "miniL.y" /* yacc.c:1646  */
    {}
#line 2234 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 673 "miniL.y" /* yacc.c:1646  */
    {
                            if(((yyvsp[-1].identifierVal) != NULL) && ((yyvsp[-1].identifierVal)[0] == '\0')) //if 'nots' is empty
                            {
                              strcpy((yyval.attr).name, (yyvsp[-1].identifierVal));
                            }
                            else
                            {
                              string out = makeTemp();
                              strcpy((yyval.attr).name, out.c_str());
                              inCode << (yyvsp[-1].identifierVal) << "" << out << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            }
                          }
#line 2251 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 688 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            strcpy((yyval.attr).name, out.c_str());
                            inCode << ". " << out << endl;
                            inCode << (yyvsp[-1].identifierVal) << " " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;                                            
                          }
#line 2262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 696 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            strcpy((yyval.attr).name, out.c_str());
                            inCode << ". " << out << endl;
                            inCode << "= " << out << ", " << "1" << endl;
                          }
#line 2273 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 704 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            strcpy((yyval.attr).name, out.c_str());
                            inCode << ". " << out << endl;
                            inCode << "= " << out << ", " << "0" << endl;
                          }
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 712 "miniL.y" /* yacc.c:1646  */
    {
                            strcpy((yyval.attr).name, (yyvsp[-1].attr).name);
                          }
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 718 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("! ");}
#line 2298 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 721 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("");}
#line 2304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 725 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("==");}
#line 2310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 728 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("!=");}
#line 2316 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 731 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("<");}
#line 2322 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 734 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>(">");}
#line 2328 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 737 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>("<=");}
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 740 "miniL.y" /* yacc.c:1646  */
    {(yyval.identifierVal) = const_cast<char*>(">=");}
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 744 "miniL.y" /* yacc.c:1646  */
    {
                            strcpy((yyval.attr).name,(yyvsp[0].attr).name);
                            (yyval.attr).type = (yyvsp[0].attr).type;
                          }
#line 2349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 750 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "+ " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2360 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 758 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "- " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2371 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 767 "miniL.y" /* yacc.c:1646  */
    {
                            strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                            (yyval.attr).type = (yyvsp[0].attr).type;
                          }
#line 2380 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 773 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "* " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 781 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "/ " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2402 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 789 "miniL.y" /* yacc.c:1646  */
    {
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "% " << out << ", " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2413 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 798 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].attr).value;
                            (yyval.attr).type = (yyvsp[0].attr).type;
                            if ((yyvsp[0].attr).type == 0) 
                            {
                              strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                              strcpy((yyval.attr).ind, (yyval.attr).name);
                            }
                            else if ((yyvsp[0].attr).type == 1) 
                            {
                              string out = makeTemp();
                              strcpy((yyval.attr).name, out.c_str());
                              inCode << ". " <<  const_cast<char*>((yyval.attr).name) << endl;
                              inCode << "=[] " << const_cast<char*>((yyval.attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).ind) << endl;
                            }
                          }
#line 2434 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 816 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].attr).value * -1;
                            if ((yyvsp[0].attr).type == 0) 
                            {
                              strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                              strcpy((yyval.attr).ind, (yyval.attr).name);
                              (yyval.attr).type = 0;

                              string minus = makeTemp();
                              inCode << ". " << minus << endl;
                              inCode << "= " << minus << ", " << "0" << endl;

                              string num = makeTemp();
                              inCode << ". " << num << endl;
                              inCode << "= " << num << ", " << (yyvsp[0].attr).name << endl;

                              string out = makeTemp();
                              strcpy((yyval.attr).name , out.c_str());
                              inCode << ". " << const_cast<char*>((yyval.attr).name) << endl;
                              inCode << "- " << const_cast<char*>((yyval.attr).name) << ", " << minus << ", " << num << endl;  
                            }
                            else if ((yyvsp[0].attr).type == 1) 
                            {

                              string minus = makeTemp();
                              inCode << ". " << minus << endl;
                              inCode << "= " << minus << ", " << "0" << endl;

                              string num = makeTemp();
                              inCode << ". " <<  num << endl;
                              inCode << "=[] " << num << ", " << const_cast<char*>((yyvsp[0].attr).name) << ", " << const_cast<char*>((yyvsp[0].attr).ind) << endl;

                              string out = makeTemp();
                              strcpy((yyval.attr).name , out.c_str());
                              strcpy((yyval.attr).ind, (yyval.attr).name);
                              (yyval.attr).type = 0;
                              inCode << ". " << const_cast<char*>((yyval.attr).name) << endl;
                              inCode << "- " << const_cast<char*>((yyval.attr).name) << ", " << minus << ", " << num << endl;  
                            }                       
                          }
#line 2479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 858 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].attr).value;
                            (yyval.attr).type = (yyvsp[0].attr).type;
                            strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                            strcpy((yyval.attr).ind, (yyvsp[0].attr).ind);                            
                          }
#line 2490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 866 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].attr).value * -1;
                            (yyval.attr).type = (yyvsp[0].attr).type;
                            strcpy((yyval.attr).name, (yyvsp[0].attr).name);
                            strcpy((yyval.attr).ind, (yyvsp[0].attr).ind);  

                            string minus = makeTemp();
                            inCode << ". " << minus << endl;
                            inCode << "= " << minus << ", " << "0" << endl;

                            string num = makeTemp();
                            inCode << ". " << num << endl;
                            inCode << "= " << num << ", " << (yyvsp[0].attr).value << endl;

                            string out = makeTemp();
                            strcpy((yyval.attr).name , out.c_str());
                            inCode << ". " << const_cast<char*>((yyval.attr).name) << endl;
                            inCode << "- " << const_cast<char*>((yyval.attr).name) << ", " << minus << ", " << num << endl;  
                          }
#line 2514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 887 "miniL.y" /* yacc.c:1646  */
    {
                            strcpy((yyval.attr).name, (yyvsp[-1].attr).name);
                          }
#line 2522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 892 "miniL.y" /* yacc.c:1646  */
    {
                            string minus = makeTemp();
                            inCode << ". " << minus << endl;
                            inCode << "= " << minus << ", " << "0" << endl;

                            string num = makeTemp();
                            inCode << ". " << num << endl;
                            inCode << "= " << num << ", " << (yyvsp[-1].attr).name << endl;

                            string out = makeTemp();
                            strcpy((yyval.attr).name , out.c_str());
                            inCode << ". " << const_cast<char*>((yyval.attr).name) << endl;
                            inCode << "- " << const_cast<char*>((yyval.attr).name) << ", " << minus << ", " << num << endl;  
                          }
#line 2541 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 908 "miniL.y" /* yacc.c:1646  */
    {
                            string name = string((yyvsp[-3].attr).name);
                            checkFunctionExists(name);
                            while (!expStack.empty())
                            {
                              inCode << "param " << expStack.top() << endl;
                              expStack.pop();
                            }
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "call " << const_cast<char*>((yyvsp[-3].attr).name) << ", " << out << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 923 "miniL.y" /* yacc.c:1646  */
    {
                            string name = string((yyvsp[-2].attr).name);
                            checkFunctionExists(name);
                            string out = makeTemp();
                            inCode << ". " << out << endl;
                            inCode << "call " << const_cast<char*>((yyvsp[-2].attr).name) << ", " << out << endl;
                            strcpy((yyval.attr).name, out.c_str());
                          }
#line 2572 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 934 "miniL.y" /* yacc.c:1646  */
    {
                            expStack.push((yyvsp[0].attr).name);
                          }
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 939 "miniL.y" /* yacc.c:1646  */
    {
                            expStack.push((yyvsp[0].attr).name);
                          }
#line 2588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 945 "miniL.y" /* yacc.c:1646  */
    {
                            string name = string((yyvsp[-3].attr).name);
                            checkSymbolExists(name);
                            if(checkSymbolIsOfType(name, ARR))
                            {
                              if ((yyvsp[-1].attr).type == 1) 
                              {
                                string out = makeTemp();
                                (yyval.attr).type = 1;
                                strcpy((yyval.attr).ind, out.c_str());
                                strcpy((yyval.attr).name, (yyvsp[-3].attr).name);
                                inCode << ". " << out << endl; 
                                inCode << "=[] " << out << ", " << const_cast<char*>((yyvsp[-1].attr).name) << ", " << const_cast<char*>((yyvsp[-1].attr).ind) << endl;
                              }
                              else
                              {
                                strcpy((yyval.attr).name,(yyvsp[-3].attr).name);
                                (yyval.attr).type = 1;
                                strcpy((yyval.attr).ind, (yyvsp[-1].attr).name);
                              }
                            }
                          }
#line 2615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 969 "miniL.y" /* yacc.c:1646  */
    {
                            string name = string((yyvsp[0].attr).name);
                            checkSymbolExists(name);
                            if(checkSymbolIsOfType(name, INT))
                            {
                             	strcpy((yyval.attr).name , (yyvsp[0].attr).name);	
                              (yyval.attr).type = 0;
                            }
                          }
#line 2629 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 981 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = 42;
                            (yyval.attr).type = 0;
                            char chars[100];
                            sprintf(chars, "%d", (yyvsp[0].identifierVal));
                            strcpy((yyval.attr).name, (yyvsp[0].identifierVal));
                            strcpy((yyval.attr).name, chars);
                            strcpy((yyval.attr).ind, (yyval.attr).name);
                          }
#line 2643 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 993 "miniL.y" /* yacc.c:1646  */
    {
                            (yyval.attr).value = (yyvsp[0].numberVal);
                            (yyval.attr).type = 0;
                            char chars[100];
                            sprintf(chars, "%d", (yyvsp[0].numberVal));
                            strcpy((yyval.attr).name, chars);
                            strcpy((yyval.attr).ind, (yyval.attr).name);
                          }
#line 2656 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2660 "y.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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
                      yytoken, &yylval, &yylloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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
  return yyresult;
}
#line 1003 "miniL.y" /* yacc.c:1906  */
 

int main(int argc, char ** argv) 
{
	if (argc >= 2) 
  {
		yyin = fopen(argv[1], "r");
		if (yyin == NULL) 
    {
			yyin = stdin;
		}
	}
	else 
  {
		yyin = stdin;
	}
	yyparse();

  if(mainExists==0)
  {
    yyerror("Error: main function was not able to be found.");
  }
  else if (mainExists==1)
  {
    if(mainDuplicateExists==1)
    {
          yyerror("Error: Dupluicate main functions found.");
    }
  }

  ofstream file;
  file.open("out.mil");
  file << outCode.str();
  file.close();
	return 0;
}
