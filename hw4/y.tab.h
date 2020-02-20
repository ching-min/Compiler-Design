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
extern YYSTYPE yylval;
