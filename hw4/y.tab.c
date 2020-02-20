#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

/**
 * Introduction to Compiler Design by Prof. Yi Ping You
 * Project 3 YACC sample
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "symtab.h"
#include "semcheck.h"

/*#include "test.h"*/

int yydebug;

extern int linenum;		/* declared in lex.l */
extern FILE *yyin;		/* declared by lex */
extern char *yytext;		/* declared by lex */
extern char buf[256];		/* declared in lex.l */
extern int yylex(void);
int yyerror(char* );
FILE *fout;
int scope = 0;
int r=0;
int Opt_D = 1;			/* symbol table dump option */
char fileName[256];
int stackLabel=1;
int stack_num=0;
int count=0;
char decl_type(struct PType *n){
	if(n->type==VOID_t){return 'V';}
	else if(n->type==INTEGER_t){return 'I';}
	else if(n->type==BOOLEAN_t){return 'Z';}
	else if(n->type==STRING_t){return 'C';}
	else if(n->type==REAL_t){return 'F';}
}

struct SymTable *symbolTable;	/* main symbol table*/

__BOOLEAN paramError;			/* indicate is parameter have any error?*/

struct PType *funcReturn;		/* record function's return type, used at 'return statement' production rule*/

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	int intVal;
	float realVal;
/*	boolean boolVal;*/
	/*__BOOLEAN booleanVal;*/
	char *lexeme;
	struct idNode_sem *id;
	/*SEMTYPE type;*/
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	/*struct var_ref_sem *varRef; */
	struct expr_sem_node *exprNode;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define ARRAY 257
#define BEG 258
#define BOOLEAN 259
#define DEF 260
#define DO 261
#define ELSE 262
#define END 263
#define FALSE 264
#define FOR 265
#define INTEGER 266
#define IF 267
#define OF 268
#define PRINT 269
#define READ 270
#define REAL 271
#define RETURN 272
#define STRING 273
#define THEN 274
#define TO 275
#define TRUE 276
#define VAR 277
#define WHILE 278
#define OP_ADD 279
#define OP_SUB 280
#define OP_MUL 281
#define OP_DIV 282
#define OP_MOD 283
#define OP_ASSIGN 284
#define OP_EQ 285
#define OP_NE 286
#define OP_GT 287
#define OP_LT 288
#define OP_GE 289
#define OP_LE 290
#define OP_AND 291
#define OP_OR 292
#define OP_NOT 293
#define MK_COMMA 294
#define MK_COLON 295
#define MK_SEMICOLON 296
#define MK_LPAREN 297
#define MK_RPAREN 298
#define MK_LB 299
#define MK_RB 300
#define ID 301
#define INT_CONST 302
#define FLOAT_CONST 303
#define SCIENTIFIC 304
#define STR_CONST 305
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
   26,    0,   31,   27,   28,   28,   32,   32,   33,   33,
   33,    2,    2,    2,    2,    2,    2,    2,    2,    2,
   29,   29,   34,   34,   36,   37,   38,   39,   35,    9,
    9,   40,    8,    8,    7,    1,    1,    6,    6,    3,
    3,    4,    4,    4,    4,    5,   24,   24,   41,   41,
   41,   41,   41,   41,   41,   48,   30,   49,   49,   50,
   50,   42,   51,   42,   52,   42,   47,   54,   43,   43,
   53,   44,   55,   56,   57,   45,   25,   25,   46,   19,
   19,   18,   18,   11,   11,   12,   12,   13,   13,   14,
   14,   23,   23,   23,   23,   23,   23,   58,   15,   15,
   22,   22,   16,   16,   21,   21,   21,   17,   17,   17,
   17,   17,   17,   17,   10,   10,   20,
};
static const short yylen[] = {                            2,
    0,    6,    0,    4,    1,    0,    2,    1,    5,    5,
    5,    1,    2,    1,    2,    1,    2,    1,    1,    1,
    1,    0,    2,    1,    0,    0,    0,    0,   13,    1,
    0,    0,    4,    1,    3,    3,    1,    2,    0,    1,
    1,    1,    1,    1,    1,    6,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    0,    5,    1,    0,    2,
    1,    4,    0,    4,    0,    4,    5,    0,    9,    6,
    1,    6,    1,    0,    0,   12,    1,    2,    3,    1,
    0,    3,    1,    3,    1,    3,    1,    2,    1,    3,
    1,    1,    1,    1,    1,    1,    1,    0,    4,    1,
    1,    1,    3,    1,    1,    1,    1,    1,    2,    3,
    4,    4,    5,    1,    1,    2,    3,
};
static const short yydefred[] = {                         0,
    1,    0,    0,    0,    0,    0,    0,    0,    8,   37,
    0,    0,    0,    3,    0,   24,    7,    0,    0,    2,
   25,   56,   23,   36,    0,   44,   20,   42,   43,   45,
   19,    0,   12,   14,   16,   18,    0,    0,    0,    0,
    4,    0,    0,   47,    0,   13,   15,   17,   11,    9,
   10,    0,   34,    0,   26,    0,   48,    0,    0,    0,
    0,    0,    0,   35,   40,   41,    0,    0,    0,    0,
   63,   65,    0,    0,    0,    0,   49,   61,   50,   51,
   52,   53,   54,   55,    0,    0,    0,   33,    0,   27,
   74,    0,    0,    0,    0,  114,    0,    0,    0,   87,
   89,    0,    0,  104,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  116,   57,   60,   46,   38,    0,    0,
    0,    0,    0,   88,    0,    0,    0,    0,   94,   97,
   96,   92,   95,   93,    0,    0,  105,  106,  107,    0,
    0,    0,  115,    0,   79,    0,    0,    0,    0,    0,
    0,   56,    0,    0,    0,  110,    0,    0,   86,    0,
  101,  102,    0,  103,    0,   64,   66,    0,    0,    0,
   62,  117,   28,    0,   77,    0,  111,    0,  112,    0,
   68,    0,    0,    0,   67,    0,   78,    0,  113,    0,
   70,   72,    0,   75,    0,   29,    0,    0,    0,   69,
    0,    0,   76,
};
static const short yydgoto[] = {                          2,
   52,   96,   64,   65,   66,   90,   53,   54,   55,   97,
  147,   99,  100,  101,  102,  103,  104,  148,  149,  114,
  140,  163,  135,   45,  176,    3,    6,    7,   14,   77,
   22,    8,    9,   15,   16,   40,   61,  119,  186,   60,
   78,   79,   80,   81,   82,   83,   84,   42,   85,   86,
  106,  107,  105,  190,  110,  120,  197,  136,
};
static const short yysindex[] = {                      -259,
    0,    0, -231, -205, -221, -147, -166, -205,    0,    0,
 -242, -165, -152,    0, -166,    0,    0, -155, -244,    0,
    0,    0,    0,    0, -271,    0,    0,    0,    0,    0,
    0, -179,    0,    0,    0,    0, -144, -141, -140, -221,
    0,  -82, -149,    0,  -97,    0,    0,    0,    0,    0,
    0, -203,    0,    0,    0, -205,    0, -271, -164, -112,
  -99, -253,  -67,    0,    0,    0, -221,  -90,  -79, -163,
    0,    0, -163, -163,  -73, -281,    0,    0,    0,    0,
    0,    0,    0,    0,  -56, -253, -164,    0, -164,    0,
    0, -143, -163, -163,  -69,    0,  -65,  -59,  -61,    0,
    0,   20, -177,    0,  -39, -163,  -54, -214,  -59,  -16,
 -163, -163, -163,    0,    0,    0,    0,    0,  -45,  -31,
 -163,  -41,  -65,    0, -257, -163, -163, -163,    0,    0,
    0,    0,    0,    0, -154, -159,    0,    0,    0, -154,
 -253, -198,    0, -210,    0, -253,  -59,  -37,  -40, -184,
 -266,    0, -269, -225, -163,    0,  -30,  -61,    0,    0,
    0,    0, -154,    0, -130,    0,    0,    7, -163,  -22,
    0,    0,    0,  -26,    0,    4,    0,  -18,    0, -177,
    0,   14,   22,  -59,    0,   23,    0, -269,    0, -253,
    0,    0,  -17,    0,   24,    0,   27,   18, -253,    0,
   34,   32,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0, -252,    0,    0,   41, -201,    0,    0,
    0,    0,    0,    0,   44,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   13,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -199,    0, -182,    0,    0,    0,    0,
    0, -207,    0,    0,    0,    0,    0,   29,    0,    0,
    0,    0,    0,    0, -261,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -219,    0,    0,    0,    0,
    0,    0,    0,    0, -117,    0,  -94,   46,   30,    0,
    0,   -2,  -48,    0,    0,    0,    0,    0,   53,    0,
   25,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -117,  -71,    0,    0,   25,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -188,    0,    0,    0,    0, -207, -183,   31,    0,    0,
    0,    0,    0,    0,   25,    0,    0,   42,    0,   21,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -25,
    0,    0,    0, -180,    0,    0,    0,    0,    0, -207,
    0,    0,    0,    0,    0,    0,    0,    0, -207,    0,
    0,    0,    0,
};
static const short yygindex[] = {                         0,
  313,  308,   40,  312,  314,    0,  265,    0,    0,  -62,
  -66,  208,  -91,    0,  202,  176,  201,    0, -105,    0,
    0,    0,    0,  285,  156,    0,    0,  289,    0,  -21,
    0,    0,  338,    0,  332,    0,    0,    0,    0,    0,
  262,    0,    0,    0,    0,    0,    0,    0, -136,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 348
static const short yytable[] = {                         76,
   41,  124,  112,   98,  165,    6,  108,  109,   43,  168,
  174,   69,   25,   70,   26,   71,   72,  113,   73,   27,
  157,   28,  115,   76,   74,  127,   29,  125,   30,  123,
   44,   31,  175,  172,  127,   32,  159,  115,   56,  142,
  156,    1,   58,   58,  144,  150,  151,   75,    6,  178,
   56,   18,   19,  195,  154,   59,    5,   33,   34,   35,
   36,    5,  201,    5,    4,    5,  127,    5,    5,   56,
    5,    5,  177,   59,   59,    6,    5,  127,   76,   10,
    6,  145,    6,   76,    6,  167,    6,    6,  113,    6,
   18,   59,   25,  127,   26,    6,   32,  166,   30,    5,
   27,   28,  184,  137,  138,  139,   29,  127,   30,   27,
   83,  171,   31,   82,   83,   12,   92,   82,    6,  161,
  162,   31,   46,   47,   48,   92,  117,   76,  118,   93,
  173,  181,  182,   94,   13,   20,   76,   95,   33,   34,
   35,   36,   94,  115,   21,   24,   95,   33,   34,   35,
   36,   49,   57,  121,   50,   51,  115,  122,   46,   47,
   48,  115,  115,  115,  115,  115,  108,  115,  115,  115,
  115,  115,  115,  115,  115,   56,  115,   58,  115,  108,
  115,  115,  115,   67,  108,  108,  108,  108,  108,  109,
  108,  108,  108,  108,  108,  108,  108,  108,   68,  108,
   87,  108,  109,  108,   89,  108,  115,  109,  109,  109,
  109,  109,  100,  109,  109,  109,  109,  109,  109,  109,
  109,   91,  109,  111,  109,  100,  109,  126,  109,  128,
  100,  100,  127,  113,  141,   99,  100,  100,  100,  100,
  100,  100,  100,  100,  146,  100,  143,  100,   99,  100,
  152,  100,  153,   99,   99,  155,  169,  170,   91,   99,
   99,   99,   99,   99,   99,   99,   99,  179,   99,  183,
   99,   91,   99,  185,   99,  187,   98,   98,  188,  189,
  191,   90,  192,  196,  200,  193,  198,  199,   91,   91,
   85,   91,  203,   91,   90,   91,  202,   91,   22,   98,
   98,   21,   84,   85,  129,  130,  131,  132,  133,  134,
   31,   90,   90,   73,   90,   84,   90,   11,   90,   71,
   90,   85,   81,   85,   39,   85,   37,   85,   80,   85,
   38,   88,   39,   84,  158,   84,  160,   84,  180,   84,
  164,   84,   63,  194,   62,   17,   23,  116,
};
static const short yycheck[] = {                         62,
   22,   93,  284,   70,  141,  258,   73,   74,  280,  146,
  280,  265,  257,  267,  259,  269,  270,  299,  272,  264,
  126,  266,  284,   86,  278,  292,  271,   94,  273,   92,
  302,  276,  302,  300,  292,  280,  128,  299,  258,  106,
  298,  301,  262,  263,  107,  112,  113,  301,  301,  155,
  258,  294,  295,  190,  121,  263,  258,  302,  303,  304,
  305,  263,  199,  265,  296,  267,  292,  269,  270,  258,
  272,  277,  298,  262,  263,  258,  278,  292,  141,  301,
  263,  296,  265,  146,  267,  296,  269,  270,  299,  272,
  294,  295,  257,  292,  259,  278,  296,  296,  298,  301,
  264,  266,  169,  281,  282,  283,  271,  292,  273,  264,
  294,  296,  276,  294,  298,  263,  280,  298,  301,  279,
  280,  276,  302,  303,  304,  280,   87,  190,   89,  293,
  152,  262,  263,  297,  301,  301,  199,  301,  302,  303,
  304,  305,  297,  261,  297,  301,  301,  302,  303,  304,
  305,  296,  302,  297,  296,  296,  274,  301,  302,  303,
  304,  279,  280,  281,  282,  283,  261,  285,  286,  287,
  288,  289,  290,  291,  292,  258,  294,  275,  296,  274,
  298,  299,  300,  296,  279,  280,  281,  282,  283,  261,
  285,  286,  287,  288,  289,  290,  291,  292,  298,  294,
  268,  296,  274,  298,  295,  300,  263,  279,  280,  281,
  282,  283,  261,  285,  286,  287,  288,  289,  290,  291,
  292,  301,  294,  297,  296,  274,  298,  297,  300,  291,
  279,  280,  292,  299,  274,  261,  285,  286,  287,  288,
  289,  290,  291,  292,  261,  294,  301,  296,  274,  298,
  296,  300,  284,  279,  280,  297,  294,  298,  261,  285,
  286,  287,  288,  289,  290,  291,  292,  298,  294,  263,
  296,  274,  298,  296,  300,  302,  279,  280,  275,  298,
  267,  261,  261,  301,  267,  263,  263,  261,  291,  292,
  261,  294,  261,  296,  274,  298,  263,  300,  258,  279,
  280,  258,  261,  274,  285,  286,  287,  288,  289,  290,
  298,  291,  292,  261,  294,  274,  296,    5,  298,  274,
  300,  292,  298,  294,  296,  296,   19,  298,  298,  300,
   19,   67,   19,  292,  127,  294,  135,  296,  163,  298,
  140,  300,   58,  188,   56,    8,   15,   86,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 305
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ARRAY","BEG","BOOLEAN","DEF",
"DO","ELSE","END","FALSE","FOR","INTEGER","IF","OF","PRINT","READ","REAL",
"RETURN","STRING","THEN","TO","TRUE","VAR","WHILE","OP_ADD","OP_SUB","OP_MUL",
"OP_DIV","OP_MOD","OP_ASSIGN","OP_EQ","OP_NE","OP_GT","OP_LT","OP_GE","OP_LE",
"OP_AND","OP_OR","OP_NOT","MK_COMMA","MK_COLON","MK_SEMICOLON","MK_LPAREN",
"MK_RPAREN","MK_LB","MK_RB","ID","INT_CONST","FLOAT_CONST","SCIENTIFIC",
"STR_CONST","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : program",
"$$1 :",
"program : ID $$1 MK_SEMICOLON program_body END ID",
"$$2 :",
"program_body : opt_decl_list opt_func_decl_list $$2 compound_stmt",
"opt_decl_list : decl_list",
"opt_decl_list :",
"decl_list : decl_list decl",
"decl_list : decl",
"decl : VAR id_list MK_COLON scalar_type MK_SEMICOLON",
"decl : VAR id_list MK_COLON array_type MK_SEMICOLON",
"decl : VAR id_list MK_COLON literal_const MK_SEMICOLON",
"literal_const : INT_CONST",
"literal_const : OP_SUB INT_CONST",
"literal_const : FLOAT_CONST",
"literal_const : OP_SUB FLOAT_CONST",
"literal_const : SCIENTIFIC",
"literal_const : OP_SUB SCIENTIFIC",
"literal_const : STR_CONST",
"literal_const : TRUE",
"literal_const : FALSE",
"opt_func_decl_list : func_decl_list",
"opt_func_decl_list :",
"func_decl_list : func_decl_list func_decl",
"func_decl_list : func_decl",
"$$3 :",
"$$4 :",
"$$5 :",
"$$6 :",
"func_decl : ID MK_LPAREN $$3 opt_param_list $$4 MK_RPAREN opt_type $$5 MK_SEMICOLON compound_stmt $$6 END ID",
"opt_param_list : param_list",
"opt_param_list :",
"$$7 :",
"param_list : param_list $$7 MK_SEMICOLON param",
"param_list : param",
"param : id_list MK_COLON type",
"id_list : id_list MK_COMMA ID",
"id_list : ID",
"opt_type : MK_COLON type",
"opt_type :",
"type : scalar_type",
"type : array_type",
"scalar_type : INTEGER",
"scalar_type : REAL",
"scalar_type : BOOLEAN",
"scalar_type : STRING",
"array_type : ARRAY array_index TO array_index OF type",
"array_index : INT_CONST",
"array_index : OP_SUB INT_CONST",
"stmt : compound_stmt",
"stmt : simple_stmt",
"stmt : cond_stmt",
"stmt : while_stmt",
"stmt : for_stmt",
"stmt : return_stmt",
"stmt : proc_call_stmt",
"$$8 :",
"compound_stmt : $$8 BEG opt_decl_list opt_stmt_list END",
"opt_stmt_list : stmt_list",
"opt_stmt_list :",
"stmt_list : stmt_list stmt",
"stmt_list : stmt",
"simple_stmt : var_ref OP_ASSIGN boolean_expr MK_SEMICOLON",
"$$9 :",
"simple_stmt : PRINT $$9 boolean_expr MK_SEMICOLON",
"$$10 :",
"simple_stmt : READ $$10 var_ref MK_SEMICOLON",
"proc_call_stmt : ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON",
"$$11 :",
"cond_stmt : IF condition THEN opt_stmt_list ELSE $$11 opt_stmt_list END IF",
"cond_stmt : IF condition THEN opt_stmt_list END IF",
"condition : boolean_expr",
"while_stmt : WHILE condition_while DO opt_stmt_list END DO",
"condition_while : boolean_expr",
"$$12 :",
"$$13 :",
"for_stmt : FOR ID $$12 OP_ASSIGN loop_param TO loop_param $$13 DO opt_stmt_list END DO",
"loop_param : INT_CONST",
"loop_param : OP_SUB INT_CONST",
"return_stmt : RETURN boolean_expr MK_SEMICOLON",
"opt_boolean_expr_list : boolean_expr_list",
"opt_boolean_expr_list :",
"boolean_expr_list : boolean_expr_list MK_COMMA boolean_expr",
"boolean_expr_list : boolean_expr",
"boolean_expr : boolean_expr OP_OR boolean_term",
"boolean_expr : boolean_term",
"boolean_term : boolean_term OP_AND boolean_factor",
"boolean_term : boolean_factor",
"boolean_factor : OP_NOT boolean_factor",
"boolean_factor : relop_expr",
"relop_expr : expr rel_op expr",
"relop_expr : expr",
"rel_op : OP_LT",
"rel_op : OP_LE",
"rel_op : OP_EQ",
"rel_op : OP_GE",
"rel_op : OP_GT",
"rel_op : OP_NE",
"$$14 :",
"expr : expr $$14 add_op term",
"expr : term",
"add_op : OP_ADD",
"add_op : OP_SUB",
"term : term mul_op factor",
"term : factor",
"mul_op : OP_MUL",
"mul_op : OP_DIV",
"mul_op : OP_MOD",
"factor : var_ref",
"factor : OP_SUB var_ref",
"factor : MK_LPAREN boolean_expr MK_RPAREN",
"factor : OP_SUB MK_LPAREN boolean_expr MK_RPAREN",
"factor : ID MK_LPAREN opt_boolean_expr_list MK_RPAREN",
"factor : OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN",
"factor : literal_const",
"var_ref : ID",
"var_ref : var_ref dim",
"dim : MK_LB boolean_expr MK_RB",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;

int yyerror( char *msg )
{
	(void) msg;
	fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );
	exit(-1);
}


#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
	{
			  struct PType *pType = createPType( VOID_t );
			  struct SymNode *newNode = createProgramNode( yystack.l_mark[0].lexeme, scope, pType );
			  insertTab( symbolTable, newNode );

			  if( strcmp(fileName,yystack.l_mark[0].lexeme) ) {
				fprintf( stdout, "########## Error at Line#%d: program beginning ID inconsist with file name ########## \n", linenum );
			  }
			  char str[256];
			  strcpy(str,yystack.l_mark[0].lexeme);
			  char *str2 = ".j";
			  strcat (str,str2);
			  fout = fopen(str,"w");
			  fprintf(fout,";%s\n",str);
			  fprintf(fout,".class public %s\n",yystack.l_mark[0].lexeme);
			  fprintf(fout,".super java/lang/Object\n");
			  fprintf(fout,".field public static _sc Ljava/util/Scanner;\n");
			}
break;
case 2:
	{
			  if( strcmp(yystack.l_mark[-5].lexeme, yystack.l_mark[0].lexeme) ) { fprintf( stdout, "########## Error at Line #%d: %s", linenum,"Program end ID inconsist with the beginning ID ########## \n"); }
			  if( strcmp(fileName,yystack.l_mark[0].lexeme) ) {
				 fprintf( stdout, "########## Error at Line#%d: program end ID inconsist with file name ########## \n", linenum );
			  }
			  /* dump symbol table*/
			  if( Opt_D == 1 )
				printSymTable( symbolTable, scope );
			  fprintf(fout,"	return\n");
			  fprintf(fout,".end method\n");
			  fclose(fout);
			}
break;
case 3:
	{
						fprintf(fout,".method public static main([Ljava/lang/String;)V\n");
						fprintf(fout,"	.limit stack 1000\n");
						fprintf(fout,"	.limit locals 1000\n" );
						fprintf(fout, "new java/util/Scanner\n");
						fprintf(fout, "dup\n");
						fprintf(fout, "getstatic java/lang/System/in Ljava/io/InputStream;\n");
						fprintf(fout, "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
						fprintf(fout, "putstatic %s/_sc Ljava/util/Scanner;\n",fileName);
						
					}
break;
case 9:
	{
			  /* insert into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yystack.l_mark[-3].id ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }/*has been declared*/
				else {
					
					newNode = createVarNode( ptr->value, scope, yystack.l_mark[-1].ptype,count );
					insertTab( symbolTable, newNode );
					if(scope==0){fprintf(fout,".field public static %s %c \n",ptr->value,decl_type(yystack.l_mark[-1].ptype));}/*not in compound */
					if(scope!=0){count++;}
				}
			  }
			  
			  deleteIdList( yystack.l_mark[-3].id );
			}
break;
case 10:
	{
			  verifyArrayType( yystack.l_mark[-3].id, yystack.l_mark[-1].ptype );
			  /* insert into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yystack.l_mark[-3].id ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( yystack.l_mark[-1].ptype->isError == __TRUE ) { }
				else if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, yystack.l_mark[-1].ptype,0 );
					insertTab( symbolTable, newNode );
					if(scope==0){fprintf(fout,".field public static %s %c\n",ptr->value,decl_type(yystack.l_mark[-1].ptype));}
				}
			  }
			  
			  deleteIdList( yystack.l_mark[-3].id );
			}
break;
case 11:
	{
			  struct PType *pType = createPType( yystack.l_mark[-1].constVal->category );
			  /* insert constants into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yystack.l_mark[-3].id ; ptr!=NULL ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createConstNode( ptr->value, scope, pType, yystack.l_mark[-1].constVal,count );
					insertTab( symbolTable, newNode );
					if(scope!=0){count++;}
				}
			  }
			 
			  
			  deleteIdList( yystack.l_mark[-3].id );
			}
break;
case 12:
	{
			  int tmp = yystack.l_mark[0].intVal;
			  yyval.constVal = createConstAttr( INTEGER_t, &tmp );
			}
break;
case 13:
	{
			  int tmp = -yystack.l_mark[0].intVal;
			  yyval.constVal = createConstAttr( INTEGER_t, &tmp );
			}
break;
case 14:
	{
			  float tmp = yystack.l_mark[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 15:
	{
			  float tmp = -yystack.l_mark[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 16:
	{
			  float tmp = yystack.l_mark[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 17:
	{
			  float tmp = -yystack.l_mark[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 18:
	{
			  yyval.constVal = createConstAttr( STRING_t, yystack.l_mark[0].lexeme );
			}
break;
case 19:
	{
			  __BOOLEAN tmp = __TRUE;
			  yyval.constVal = createConstAttr( BOOLEAN_t, &tmp );
			}
break;
case 20:
	{
			  __BOOLEAN tmp = __FALSE;
			  yyval.constVal = createConstAttr( BOOLEAN_t, &tmp );
			}
break;
case 25:
	{count=0;fprintf(fout,".method public static %s(",yystack.l_mark[-1].lexeme);}
break;
case 26:
	{
			  /* check and insert parameters into symbol table*/
			  paramError = insertParamIntoSymTable( symbolTable, yystack.l_mark[0].par, scope+1 );
			}
break;
case 27:
	{
			  /* check and insert function into symbol table*/
			  if( paramError == __TRUE ) {
			  	printf("--- param(s) with several fault!! ---\n");
			  } else {
				insertFuncIntoSymTable( symbolTable, yystack.l_mark[-6].lexeme, yystack.l_mark[-3].par, yystack.l_mark[0].ptype, scope );
			  }
			  
			  fprintf(fout,")%c\n",decl_type(yystack.l_mark[0].ptype));
			  fprintf(fout,".limit stack 1000\n");
			  fprintf( fout, ".limit locals 1000\n" );
			  /*fprintf(fout,"\n");*/
			  funcReturn = yystack.l_mark[0].ptype;
			}
break;
case 28:
	{
				if(funcReturn->type==VOID_t){fprintf( fout, "return\n" );}
				else if(funcReturn->type==INTEGER_t){fprintf( fout, "ireturn\n" );}
				else if(funcReturn->type==BOOLEAN_t){fprintf( fout, "ireturn\n" );}
				else if(funcReturn->type==REAL_t){fprintf( fout, "freturn\n" );}
				fprintf( fout, ".end method\n" ); 
			  }
break;
case 29:
	{
			  /*if( strcmp($1,$13) ) {*/
				/*fprintf( stdout, "########## Error at Line #%d: the end of the functionName mismatch ########## \n", linenum );*/
			  /*}*/
			  funcReturn = 0;
			}
break;
case 30:
	{ yyval.par = yystack.l_mark[0].par; }
break;
case 31:
	{ yyval.par = 0; }
break;
case 32:
	{fprintf( fout, "," );}
break;
case 33:
	{
			  
			  param_sem_addParam( yystack.l_mark[-3].par, yystack.l_mark[0].par );
			  yyval.par = yystack.l_mark[-3].par;
			}
break;
case 34:
	{ yyval.par = yystack.l_mark[0].par; }
break;
case 35:
	{ 
					yyval.par = createParam( yystack.l_mark[-2].id, yystack.l_mark[0].ptype ); 
					struct idNode_sem *p;
					for(p=yystack.l_mark[-2].id;p!=0;p=p->next){fprintf(fout,"%c",decl_type(yystack.l_mark[0].ptype));}
					if(scope!=0){count++;}
				}
break;
case 36:
	{
			  idlist_addNode( yystack.l_mark[-2].id, yystack.l_mark[0].lexeme );
			  yyval.id = yystack.l_mark[-2].id;
			  /*count++;*/
			}
break;
case 37:
	{ yyval.id = createIdList(yystack.l_mark[0].lexeme); /*count++;*/}
break;
case 38:
	{ yyval.ptype = yystack.l_mark[0].ptype; }
break;
case 39:
	{ yyval.ptype = createPType( VOID_t ); }
break;
case 40:
	{ yyval.ptype = yystack.l_mark[0].ptype; }
break;
case 41:
	{ yyval.ptype = yystack.l_mark[0].ptype; }
break;
case 42:
	{ yyval.ptype = createPType( INTEGER_t ); }
break;
case 43:
	{ yyval.ptype = createPType( REAL_t ); }
break;
case 44:
	{ yyval.ptype = createPType( BOOLEAN_t ); }
break;
case 45:
	{ yyval.ptype = createPType( STRING_t ); }
break;
case 46:
	{
				verifyArrayDim( yystack.l_mark[0].ptype, yystack.l_mark[-4].intVal, yystack.l_mark[-2].intVal );
				increaseArrayDim( yystack.l_mark[0].ptype, yystack.l_mark[-4].intVal, yystack.l_mark[-2].intVal );
				yyval.ptype = yystack.l_mark[0].ptype;
			}
break;
case 47:
	{ yyval.intVal = yystack.l_mark[0].intVal; }
break;
case 48:
	{ yyval.intVal = -yystack.l_mark[0].intVal; }
break;
case 56:
	{ 
			  scope++;
			}
break;
case 57:
	{ 
			  /* print contents of current scope*/
			  if( Opt_D == 1 )
			  	printSymTable( symbolTable, scope );
			  deleteScope( symbolTable, scope );	/* leave this scope, delete...*/
			  scope--; 
			}
break;
case 62:
	{
			  /* check if LHS exists*/
			  __BOOLEAN flagLHS = verifyExistence( symbolTable, yystack.l_mark[-3].exprs, scope, __TRUE );
			  /* id RHS is not dereferenced, check and deference*/
			  __BOOLEAN flagRHS = __TRUE;
			  if( yystack.l_mark[-1].exprs->isDeref == __FALSE ) {
				flagRHS = verifyExistence( symbolTable, yystack.l_mark[-1].exprs, scope, __FALSE );
			  }
			  /* if both LHS and RHS are exists, verify their type*/
			   if( yystack.l_mark[-1].exprs->isDeref == __FALSE ) {
				flagRHS = verifyExistence( symbolTable, yystack.l_mark[-1].exprs, scope, __FALSE );
			  }
			  /* if both LHS and RHS are exists, verify their type*/
			  
			  struct SymNode *node1 = lookupSymbol( symbolTable, yystack.l_mark[-3].exprs -> varRef -> id, scope, __FALSE );
			  if(node1->scope==0){/*global*/
				  fprintf(fout,"putstatic %s/%s ",fileName,yystack.l_mark[-3].exprs -> varRef -> id);
				  if(node1->type->type==INTEGER_t){fprintf(fout,"I\n");}
				  else if(node1->type->type==BOOLEAN_t){fprintf(fout,"Z\n");}
				  else if(node1->type->type==REAL_t){fprintf(fout,"F\n");}
			  }else{/*local*/
				  if(yystack.l_mark[-1].exprs->pType->type==INTEGER_t&&yystack.l_mark[-3].exprs->pType->type==REAL_t){
					  fprintf(fout,"i2f\n");
				  }
				  if(yystack.l_mark[-3].exprs->pType->type==INTEGER_t&&yystack.l_mark[-1].exprs->pType->type==REAL_t){
					  fprintf(fout,"f2i\n");
				  }
				  if(node1->type->type==INTEGER_t||node1->type->type==BOOLEAN_t){fprintf(fout,"istore %d\n",node1->count);}
				  else{fprintf(fout,"fstore %d\n",node1->count);}
			  }
			  
			  if(yystack.l_mark[-1].exprs -> pType -> type==INTEGER_t)
			  {
				if(node1->type->type==REAL_t)
					fprintf( fout, "	i2f\n" );
			  }
 
			  flagLHS = verifyExistence( symbolTable, yystack.l_mark[-3].exprs, scope, __TRUE);
			  if( flagLHS==__TRUE && flagRHS==__TRUE )
				verifyAssignmentTypeMatch( yystack.l_mark[-3].exprs, yystack.l_mark[-1].exprs );
			}
break;
case 63:
	{
				fprintf(fout,"	getstatic java/lang/System/out Ljava/io/PrintStream;\n" );
			}
break;
case 64:
	{ 
				fprintf(fout,"	invokevirtual java/io/PrintStream/print(" );
				/*struct SymNode *node1= lookupSymbol( symbolTable, $3 -> varRef -> id, scope, __FALSE );*/
				if(yystack.l_mark[-1].exprs -> pType -> type==INTEGER_t){fprintf(fout,"I" );}
				else if(yystack.l_mark[-1].exprs -> pType -> type==REAL_t){fprintf(fout,"F" );}
				else if(yystack.l_mark[-1].exprs -> pType -> type==BOOLEAN_t){fprintf(fout,"Z" );}
				else{fprintf(fout,"Ljava/lang/String;" );}
				fprintf(fout,")V \n" );
			}
break;
case 65:
	{r=1;fprintf(fout,"getstatic %s/_sc Ljava/util/Scanner; \n", fileName );}
break;
case 66:
	{ 
				struct SymNode *node1= lookupSymbol( symbolTable, yystack.l_mark[-1].exprs -> varRef -> id, scope, __FALSE );
				fprintf(fout,"invokevirtual java/util/Scanner/");
				if(node1->type->type==INTEGER_t){fprintf(fout,"nextInt()I\n");}
				else if(node1->type->type==REAL_t){fprintf(fout,"nextFloat()F\n");}
				else if(node1->type->type==BOOLEAN_t){fprintf(fout,"nextBOOLEAN()Z\n");}
				
				if(node1->scope==0){/*global*/
					fprintf(fout,"putstatic %s/%s",fileName,yystack.l_mark[-1].exprs -> varRef -> id);					
				}else{
					if(node1->type->type==INTEGER_t||node1->type->type==BOOLEAN_t){fprintf(fout, "istore %d\n",node1->count);}
					else if(node1->type->type==REAL_t){fprintf(fout, "fstore %d\n",node1->count);}
				}
			}
break;
case 67:
	{
			  verifyFuncInvoke( yystack.l_mark[-4].lexeme, yystack.l_mark[-2].exprs, symbolTable, scope );
			}
break;
case 68:
	{fprintf(fout,"	goto L_%d\n",stackLabel-1);
			  fprintf(fout,"L_%d:\n",stackLabel-2);
			  }
break;
case 69:
	{fprintf(fout,"L_%d:\n",stackLabel-1);}
break;
case 70:
	{fprintf(fout,"L_%d:\n",stackLabel-2);}
break;
case 71:
	{ verifyBooleanExpr( yystack.l_mark[0].exprs, "if" );
				fprintf(fout,"	ifeq L_%d\n",stackLabel-2); }
break;
case 73:
	{ verifyBooleanExpr( yystack.l_mark[0].exprs, "while" ); }
break;
case 74:
	{ 
			  insertLoopVarIntoTable( symbolTable, yystack.l_mark[0].lexeme );
			}
break;
case 75:
	{
			  verifyLoopParam( yystack.l_mark[-2].intVal, yystack.l_mark[0].intVal );
			}
break;
case 76:
	{
			  popLoopVar( symbolTable );
			}
break;
case 77:
	{ yyval.intVal = yystack.l_mark[0].intVal; }
break;
case 78:
	{ yyval.intVal = -yystack.l_mark[0].intVal; }
break;
case 79:
	{
			  verifyReturnStatement( yystack.l_mark[-1].exprs, funcReturn );
			  
			}
break;
case 80:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 81:
	{ yyval.exprs = 0; }
break;
case 82:
	{
			  struct expr_sem *exprPtr;
			  for( exprPtr=yystack.l_mark[-2].exprs ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  exprPtr->next = yystack.l_mark[0].exprs;
			  yyval.exprs = yystack.l_mark[-2].exprs;
			}
break;
case 83:
	{
			  yyval.exprs = yystack.l_mark[0].exprs;
			}
break;
case 84:
	{
			  verifyAndOrOp( yystack.l_mark[-2].exprs, OR_t, yystack.l_mark[0].exprs );
			  yyval.exprs = yystack.l_mark[-2].exprs;
			  fprintf(fout,"	ior\n");
			}
break;
case 85:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 86:
	{
			  verifyAndOrOp( yystack.l_mark[-2].exprs, AND_t, yystack.l_mark[0].exprs );
			  yyval.exprs = yystack.l_mark[-2].exprs;
			  fprintf(fout,"	iand\n");
			}
break;
case 87:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 88:
	{
			  verifyUnaryNOT( yystack.l_mark[0].exprs );
			  yyval.exprs = yystack.l_mark[0].exprs;
			  fprintf(fout,"	ixor\n");
			}
break;
case 89:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 90:
	{
				if(yystack.l_mark[-2].exprs->pType->type==INTEGER_t){/*to float*/
					fprintf(fout,"i2f\n");
				}
				if(yystack.l_mark[0].exprs->pType->type==INTEGER_t){/*to float*/
					fprintf(fout,"i2f\n");
				}
				
				fprintf(fout,"fcmpl\n");
				
				if(yystack.l_mark[-1].intVal==LT_t){
					fprintf( fout, "	iflt L%d\n", stackLabel );
				}else if(yystack.l_mark[-1].intVal==LE_t){
					fprintf( fout, "	ifle L%d\n", stackLabel );
				}else if(yystack.l_mark[-1].intVal==EQ_t){
					fprintf( fout, "	ifeq L%d\n", stackLabel );
				}else if(yystack.l_mark[-1].intVal==GE_t){
					fprintf( fout, "	ifge L%d\n", stackLabel );
				}else if(yystack.l_mark[-1].intVal==GT_t){
					fprintf( fout, "	ifgt L%d\n", stackLabel );
				}else if(yystack.l_mark[-1].intVal==NE_t){
					fprintf( fout, "	ifne L%d\n", stackLabel );
				}
				
				fprintf( fout, "	iconst_0\n" );
				fprintf(fout,"	goto L%d\n", stackLabel+1);
				fprintf(fout,"	L%d:\n", stackLabel);
				fprintf( fout, "	iconst_1\n" );
				fprintf(fout,"	L%d:\n", stackLabel+1);
				stackLabel+=2;
				
			    verifyRelOp( yystack.l_mark[-2].exprs, yystack.l_mark[-1].intVal, yystack.l_mark[0].exprs );
			  yyval.exprs = yystack.l_mark[-2].exprs;
			}
break;
case 91:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 92:
	{ yyval.intVal = LT_t; }
break;
case 93:
	{ yyval.intVal = LE_t; }
break;
case 94:
	{ yyval.intVal = EQ_t; }
break;
case 95:
	{ yyval.intVal = GE_t; }
break;
case 96:
	{ yyval.intVal = GT_t; }
break;
case 97:
	{ yyval.intVal = NE_t; }
break;
case 98:
	{
				if(yystack.l_mark[0].exprs->pType->type==INTEGER_t){
					fprintf( fout, "	i2f\n");
				}
			}
break;
case 99:
	{ 
				
			 if(yystack.l_mark[0].exprs->pType->type==INTEGER_t){
					fprintf( fout, "	i2f\n");
				}
			 
			  verifyArithmeticOp( yystack.l_mark[-3].exprs, yystack.l_mark[-1].intVal, yystack.l_mark[0].exprs );
			  yyval.exprs = yystack.l_mark[-3].exprs;
			  
				if(yystack.l_mark[-1].intVal==ADD_t){fprintf( fout, "	fadd\n" );}
				else if(yystack.l_mark[-1].intVal==SUB_t){fprintf( fout, "	fsub\n" );}
				
				if(yystack.l_mark[0].exprs->pType->type==INTEGER_t&&yystack.l_mark[-3].exprs->pType->type==INTEGER_t)
					fprintf( fout, "	f2i\n");
			  
			}
break;
case 100:
	{ yyval.exprs = yystack.l_mark[0].exprs; }
break;
case 101:
	{ yyval.intVal = ADD_t; }
break;
case 102:
	{ yyval.intVal = SUB_t; }
break;
case 103:
	{	
				
			if(yystack.l_mark[0].exprs->pType->type==INTEGER_t&&yystack.l_mark[-2].exprs->pType->type==REAL_t){
					fprintf( fout, "	i2f\n");
				}
			 else if(yystack.l_mark[-2].exprs->pType->type==INTEGER_t&&yystack.l_mark[0].exprs->pType->type==REAL_t){
					
					/*fprintf( fout, "	fstore %d\n", n1->count);*/
					fprintf( fout, "	i2f\n");
					/*fprintf( fout, "	fload %d\n", n2->count);*/
				}
				if(yystack.l_mark[-1].intVal==MOD_t){verifyModOp( yystack.l_mark[-2].exprs, yystack.l_mark[0].exprs );}
			  else{verifyArithmeticOp( yystack.l_mark[-2].exprs, yystack.l_mark[-1].intVal, yystack.l_mark[0].exprs );}
			  yyval.exprs = yystack.l_mark[-2].exprs;
				if(yystack.l_mark[-2].exprs->pType->type==INTEGER_t&&yystack.l_mark[-2].exprs->pType->type==INTEGER_t){fprintf(fout,"i");}
				else{fprintf(fout,"f");}
				if(yystack.l_mark[-1].intVal==MUL_t){fprintf( fout, "mul\n" );}
				else if(yystack.l_mark[-1].intVal==DIV_t){fprintf( fout, "div\n" );}
				else if(yystack.l_mark[-1].intVal==MOD_t){fprintf( fout, "rem\n" );}
				
			}
break;
case 104:
	{ 
				
				yyval.exprs = yystack.l_mark[0].exprs; 
				if(yyval.exprs->beginningOp==SUB_t){
					if(yystack.l_mark[0].exprs->pType->type==INTEGER_t){
						fprintf( fout, "	ineg\n" );
					}else{
						fprintf( fout, "	fneg\n" );
					}
				}
			}
break;
case 105:
	{ yyval.intVal = MUL_t; }
break;
case 106:
	{ yyval.intVal = DIV_t; }
break;
case 107:
	{ yyval.intVal = MOD_t; }
break;
case 108:
	{	
				verifyExistence( symbolTable, yystack.l_mark[0].exprs, scope, __FALSE );
				yyval.exprs = yystack.l_mark[0].exprs;
				yyval.exprs->beginningOp = NONE_t;
				struct SymNode *node1 = lookupSymbol( symbolTable, yystack.l_mark[0].exprs -> varRef -> id, scope, __FALSE );
				if(node1->category==CONSTANT_t){/*constant*/
					if(node1->type->type==INTEGER_t){/*int*/
						fprintf(fout,"	sipush %d\n",node1->attribute->constVal->value.integerVal);
					}else if(node1->type->type==BOOLEAN_t){/*true false*/
						if(node1->attribute->constVal->value.booleanVal==TRUE){fprintf(fout,"iconst_1\n");}
						else if(node1->attribute->constVal->value.booleanVal==FALSE){fprintf(fout,"iconst_0\n");}
					}else if(node1->type->type==REAL_t){/*real*/
						fprintf(fout,"ldc %f\n",node1->attribute->constVal->value.realVal);
					}else if(node1->type->type==STRING_t){/*string*/
						fprintf(fout,"ldc \"%s\"\n",node1->attribute->constVal->value.stringVal);
					}
				}else if(node1->scope==0){/*global*/
					fprintf(fout,"getstatic %s/%s ",fileName,node1->name);
					if(node1->type->type==INTEGER_t){fprintf(fout,"I\n");}
					else if(node1->type->type==BOOLEAN_t){fprintf(fout,"Z\n");}
					else if(node1->type->type==REAL_t){fprintf(fout,"F\n");}
				}else if(node1->scope>0){/*local*/
					if(node1->type->type==INTEGER_t){fprintf(fout,"	iload %d\n",node1->count);}
					else if(node1->type->type==BOOLEAN_t){fprintf(fout,"	iload %d\n",node1->count);}
					else if(node1->type->type==REAL_t){fprintf(fout,"	fload %d\n",node1->count);}
					
				}
				
			  
			}
break;
case 109:
	{	
			if(r==1){
				if( verifyExistence( symbolTable, yystack.l_mark[0].exprs, scope, __TRUE ) == __TRUE ){
					verifyUnaryMinus(yystack.l_mark[0].exprs);
					
				}
				r=0;
			}else{
				if( verifyExistence( symbolTable, yystack.l_mark[0].exprs, scope, __FALSE ) == __TRUE ){
					verifyUnaryMinus(yystack.l_mark[0].exprs);
				}
			}
			  /*if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE )*/
				/*verifyUnaryMinus( $2 );*/
			  yyval.exprs = yystack.l_mark[0].exprs;
			  yyval.exprs->beginningOp = SUB_t;
			}
break;
case 110:
	{
			  yystack.l_mark[-1].exprs->beginningOp = NONE_t;
			  yyval.exprs = yystack.l_mark[-1].exprs; 
			}
break;
case 111:
	{
			  verifyUnaryMinus( yystack.l_mark[-1].exprs );
			  yyval.exprs = yystack.l_mark[-1].exprs;
			  yyval.exprs->beginningOp = SUB_t;
			}
break;
case 112:
	{	
				fprintf(fout,"	invokestatic %s/%s(",fileName,yystack.l_mark[-3].lexeme);
				struct expr_sem *p;
				for(p=yystack.l_mark[-1].exprs;p!=NULL;p=p->next){fprintf(fout,"%c",decl_type(p->pType));}
				struct SymNode *node=lookupSymbol(symbolTable,yystack.l_mark[-3].lexeme,scope,__FALSE);
				fprintf(fout,")%c\n",decl_type(node->type));
			  yyval.exprs = verifyFuncInvoke( yystack.l_mark[-3].lexeme, yystack.l_mark[-1].exprs, symbolTable, scope );
			  yyval.exprs->beginningOp = NONE_t;
			}
break;
case 113:
	{
				fprintf(fout,"	invokestatic %s/%s(",fileName,yystack.l_mark[-3].lexeme);
				struct expr_sem *p;
				for(p=yystack.l_mark[-1].exprs;p!=NULL;p=p->next){fprintf(fout,"%c",decl_type(p->pType));}
				struct SymNode *node=lookupSymbol(symbolTable,yystack.l_mark[-3].lexeme,scope,__FALSE);
				fprintf(fout,")%c\n",decl_type(node->type));
			  yyval.exprs = verifyFuncInvoke( yystack.l_mark[-3].lexeme, yystack.l_mark[-1].exprs, symbolTable, scope );
			  yyval.exprs->beginningOp = SUB_t;
			}
break;
case 114:
	{
			  yyval.exprs = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  yyval.exprs->isDeref = __TRUE;
			  yyval.exprs->varRef = 0;
			  yyval.exprs->pType = createPType( yystack.l_mark[0].constVal->category );
			  yyval.exprs->next = 0;
			  if( yystack.l_mark[0].constVal->hasMinus == __TRUE ) {
			  	yyval.exprs->beginningOp = SUB_t;
			  }
			  else {
				yyval.exprs->beginningOp = NONE_t;
			  }
			  
			  if(yyval.exprs->pType->type==INTEGER_t){
				fprintf(fout,"	ldc %d\n",yystack.l_mark[0].constVal->value.integerVal);
			  }else if(yyval.exprs->pType->type==BOOLEAN_t){
				fprintf(fout,"	iconst_%d\n",yystack.l_mark[0].constVal->value.booleanVal);
			  }else if(yyval.exprs->pType->type==STRING_t){
				fprintf(fout,"	ldc \"%s\"\n",yystack.l_mark[0].constVal->value.stringVal);
			  }else if(yyval.exprs->pType->type==REAL_t){
				fprintf(fout,"	ldc %f\n",yystack.l_mark[0].constVal->value.realVal);
			}
			}
break;
case 115:
	{
			  
			  yyval.exprs = createExprSem( yystack.l_mark[0].lexeme );
			  
			}
break;
case 116:
	{
			  increaseDim( yystack.l_mark[-1].exprs, yystack.l_mark[0].intVal );
			  yyval.exprs = yystack.l_mark[-1].exprs;
			}
break;
case 117:
	{
			  yyval.intVal = verifyArrayIndex( yystack.l_mark[-1].exprs );
			}
break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
