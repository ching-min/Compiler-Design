%{
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

//#include "test.h"

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

struct SymTable *symbolTable;	// main symbol table

__BOOLEAN paramError;			// indicate is parameter have any error?

struct PType *funcReturn;		// record function's return type, used at 'return statement' production rule

%}

%union {
	int intVal;
	float realVal;
//	boolean boolVal;
	//__BOOLEAN booleanVal;
	char *lexeme;
	struct idNode_sem *id;
	//SEMTYPE type;
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	/*struct var_ref_sem *varRef; */
	struct expr_sem_node *exprNode;
};

/* tokens */
%token ARRAY BEG BOOLEAN DEF DO ELSE END FALSE FOR INTEGER IF OF PRINT READ REAL RETURN STRING THEN TO TRUE VAR WHILE
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD OP_ASSIGN OP_EQ OP_NE OP_GT OP_LT OP_GE OP_LE OP_AND OP_OR OP_NOT
%token MK_COMMA MK_COLON MK_SEMICOLON MK_LPAREN MK_RPAREN MK_LB MK_RB

%token <lexeme>ID
%token <intVal>INT_CONST 
%token <realVal>FLOAT_CONST
%token <realVal>SCIENTIFIC
%token <lexeme>STR_CONST

%type<id> id_list
%type<constVal> literal_const
%type<ptype> type scalar_type array_type opt_type
%type<par> param param_list opt_param_list
%type<exprs> var_ref boolean_expr boolean_term boolean_factor relop_expr expr term factor boolean_expr_list opt_boolean_expr_list
%type<intVal> dim mul_op add_op rel_op array_index loop_param

/* start symbol */
%start program
%%

program			: ID
			{
			  struct PType *pType = createPType( VOID_t );
			  struct SymNode *newNode = createProgramNode( $1, scope, pType );
			  insertTab( symbolTable, newNode );

			  if( strcmp(fileName,$1) ) {
				fprintf( stdout, "########## Error at Line#%d: program beginning ID inconsist with file name ########## \n", linenum );
			  }
			  char str[256];
			  strcpy(str,$1);
			  char *str2 = ".j";
			  strcat (str,str2);
			  fout = fopen(str,"w");
			  fprintf(fout,";%s\n",str);
			  fprintf(fout,".class public %s\n",$1);
			  fprintf(fout,".super java/lang/Object\n");
			  fprintf(fout,".field public static _sc Ljava/util/Scanner;\n");
			}
			  MK_SEMICOLON 
			  program_body
			  END ID
			{
			  if( strcmp($1, $6) ) { fprintf( stdout, "########## Error at Line #%d: %s", linenum,"Program end ID inconsist with the beginning ID ########## \n"); }
			  if( strcmp(fileName,$6) ) {
				 fprintf( stdout, "########## Error at Line#%d: program end ID inconsist with file name ########## \n", linenum );
			  }
			  // dump symbol table
			  if( Opt_D == 1 )
				printSymTable( symbolTable, scope );
			  fprintf(fout,"	return\n");
			  fprintf(fout,".end method\n");
			  fclose(fout);
			}
			
			;

program_body		:opt_decl_list 
					opt_func_decl_list {
						fprintf(fout,".method public static main([Ljava/lang/String;)V\n");
						fprintf(fout,"	.limit stack 1000\n");
						fprintf(fout,"	.limit locals 1000\n" );
						fprintf(fout, "new java/util/Scanner\n");
						fprintf(fout, "dup\n");
						fprintf(fout, "getstatic java/lang/System/in Ljava/io/InputStream;\n");
						fprintf(fout, "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
						fprintf(fout, "putstatic %s/_sc Ljava/util/Scanner;\n",fileName);
						
					}
					compound_stmt
			;

opt_decl_list		: decl_list
			| /* epsilon */
			;

decl_list		: decl_list decl
			| decl
			;

decl			: VAR id_list MK_COLON scalar_type MK_SEMICOLON       /* scalar type declaration */
			{
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }//has been declared
				else {
					
					newNode = createVarNode( ptr->value, scope, $4,count );
					insertTab( symbolTable, newNode );
					if(scope==0){fprintf(fout,".field public static %s %c \n",ptr->value,decl_type($4));}//not in compound 
					if(scope!=0){count++;}
				}
			  }
			  
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON array_type MK_SEMICOLON        /* array type declaration */
			{
			  verifyArrayType( $2, $4 );
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( $4->isError == __TRUE ) { }
				else if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, $4,0 );
					insertTab( symbolTable, newNode );
					if(scope==0){fprintf(fout,".field public static %s %c\n",ptr->value,decl_type($4));}
				}
			  }
			  
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON literal_const MK_SEMICOLON     /* const declaration *//**/
			{
			  struct PType *pType = createPType( $4->category );
			  // insert constants into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=NULL ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createConstNode( ptr->value, scope, pType, $4,count );
					insertTab( symbolTable, newNode );
					if(scope!=0){count++;}
				}
			  }
			 
			  
			  deleteIdList( $2 );
			}
			;

literal_const		: INT_CONST
			{
			  int tmp = $1;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| OP_SUB INT_CONST
			{
			  int tmp = -$2;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| FLOAT_CONST
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB FLOAT_CONST
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| SCIENTIFIC 
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB SCIENTIFIC
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| STR_CONST
			{
			  $$ = createConstAttr( STRING_t, $1 );
			}
			| TRUE
			{
			  __BOOLEAN tmp = __TRUE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			| FALSE
			{
			  __BOOLEAN tmp = __FALSE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			;

opt_func_decl_list	: func_decl_list
			| /* epsilon */
			;

func_decl_list		: func_decl_list func_decl
			| func_decl
			;

func_decl		: ID MK_LPAREN{count=0;fprintf(fout,".method public static %s(",$1);} opt_param_list
			{
			  // check and insert parameters into symbol table
			  paramError = insertParamIntoSymTable( symbolTable, $4, scope+1 );
			}
			  MK_RPAREN opt_type 
			{
			  // check and insert function into symbol table
			  if( paramError == __TRUE ) {
			  	printf("--- param(s) with several fault!! ---\n");
			  } else {
				insertFuncIntoSymTable( symbolTable, $1, $4, $7, scope );
			  }
			  
			  fprintf(fout,")%c\n",decl_type($7));
			  fprintf(fout,".limit stack 1000\n");
			  fprintf( fout, ".limit locals 1000\n" );
			  //fprintf(fout,"\n");
			  funcReturn = $7;
			}
			  MK_SEMICOLON
			  compound_stmt{
				if(funcReturn->type==VOID_t){fprintf( fout, "return\n" );}
				else if(funcReturn->type==INTEGER_t){fprintf( fout, "ireturn\n" );}
				else if(funcReturn->type==BOOLEAN_t){fprintf( fout, "ireturn\n" );}
				else if(funcReturn->type==REAL_t){fprintf( fout, "freturn\n" );}
				fprintf( fout, ".end method\n" ); 
			  }
			  END ID
			{
			  //if( strcmp($1,$13) ) {
				//fprintf( stdout, "########## Error at Line #%d: the end of the functionName mismatch ########## \n", linenum );
			  //}
			  funcReturn = 0;
			}
			;

opt_param_list		: param_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }
			;

param_list		: param_list{fprintf( fout, "," );} MK_SEMICOLON param
			{
			  
			  param_sem_addParam( $1, $4 );
			  $$ = $1;
			}
			| param { $$ = $1; }
			;

param			: id_list MK_COLON type 
				{ 
					$$ = createParam( $1, $3 ); 
					struct idNode_sem *p;
					for(p=$1;p!=0;p=p->next){fprintf(fout,"%c",decl_type($3));}
					if(scope!=0){count++;}
				}
			;

id_list			: id_list MK_COMMA ID
			{
			  idlist_addNode( $1, $3 );
			  $$ = $1;
			  //count++;
			}
			| ID { $$ = createIdList($1); /*count++;*/}
			;

opt_type		: MK_COLON type { $$ = $2; }
			| /* epsilon */ { $$ = createPType( VOID_t ); }
			;

type			: scalar_type { $$ = $1; }
			| array_type { $$ = $1; }
			;

scalar_type		: INTEGER { $$ = createPType( INTEGER_t ); }
			| REAL { $$ = createPType( REAL_t ); }
			| BOOLEAN { $$ = createPType( BOOLEAN_t ); }
			| STRING { $$ = createPType( STRING_t ); }
			;

array_type		: ARRAY array_index TO array_index OF type
			{
				verifyArrayDim( $6, $2, $4 );
				increaseArrayDim( $6, $2, $4 );
				$$ = $6;
			}
			;

array_index		: INT_CONST { $$ = $1; }
			| OP_SUB INT_CONST { $$ = -$2; }
			;

stmt			: compound_stmt
			| simple_stmt
			| cond_stmt
			| while_stmt
			| for_stmt
			| return_stmt
			| proc_call_stmt
			;

compound_stmt		: 
			{ 
			  scope++;
			}
			  BEG
			  opt_decl_list
			  opt_stmt_list
			  END 
			{ 
			  // print contents of current scope
			  if( Opt_D == 1 )
			  	printSymTable( symbolTable, scope );
			  deleteScope( symbolTable, scope );	// leave this scope, delete...
			  scope--; 
			}
			;

opt_stmt_list		: stmt_list
			| /* epsilon */
			;

stmt_list		: stmt_list stmt
			| stmt
			;

simple_stmt		: var_ref OP_ASSIGN boolean_expr MK_SEMICOLON
			{
			  // check if LHS exists
			  __BOOLEAN flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE );
			  // id RHS is not dereferenced, check and deference
			  __BOOLEAN flagRHS = __TRUE;
			  if( $3->isDeref == __FALSE ) {
				flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
			  }
			  // if both LHS and RHS are exists, verify their type
			   if( $3->isDeref == __FALSE ) {
				flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
			  }
			  // if both LHS and RHS are exists, verify their type
			  
			  struct SymNode *node1 = lookupSymbol( symbolTable, $1 -> varRef -> id, scope, __FALSE );
			  if(node1->scope==0){//global
				  fprintf(fout,"putstatic %s/%s ",fileName,$1 -> varRef -> id);
				  if(node1->type->type==INTEGER_t){fprintf(fout,"I\n");}
				  else if(node1->type->type==BOOLEAN_t){fprintf(fout,"Z\n");}
				  else if(node1->type->type==REAL_t){fprintf(fout,"F\n");}
			  }else{//local
				  if($3->pType->type==INTEGER_t&&$1->pType->type==REAL_t){
					  fprintf(fout,"i2f\n");
				  }
				  if($1->pType->type==INTEGER_t&&$3->pType->type==REAL_t){
					  fprintf(fout,"f2i\n");
				  }
				  if(node1->type->type==INTEGER_t||node1->type->type==BOOLEAN_t){fprintf(fout,"istore %d\n",node1->count);}
				  else{fprintf(fout,"fstore %d\n",node1->count);}
			  }
			  
			  if($3 -> pType -> type==INTEGER_t)
			  {
				if(node1->type->type==REAL_t)
					fprintf( fout, "	i2f\n" );
			  }
 
			  flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE);
			  if( flagLHS==__TRUE && flagRHS==__TRUE )
				verifyAssignmentTypeMatch( $1, $3 );
			}
			| PRINT 
			{
				fprintf(fout,"	getstatic java/lang/System/out Ljava/io/PrintStream;\n" );
			}
			boolean_expr MK_SEMICOLON { 
				fprintf(fout,"	invokevirtual java/io/PrintStream/print(" );
				//struct SymNode *node1= lookupSymbol( symbolTable, $3 -> varRef -> id, scope, __FALSE );
				if($3 -> pType -> type==INTEGER_t){fprintf(fout,"I" );}
				else if($3 -> pType -> type==REAL_t){fprintf(fout,"F" );}
				else if($3 -> pType -> type==BOOLEAN_t){fprintf(fout,"Z" );}
				else{fprintf(fout,"Ljava/lang/String;" );}
				fprintf(fout,")V \n" );
			}
 			| READ {r=1;fprintf(fout,"getstatic %s/_sc Ljava/util/Scanner; \n", fileName );}var_ref 
			MK_SEMICOLON { 
				struct SymNode *node1= lookupSymbol( symbolTable, $3 -> varRef -> id, scope, __FALSE );
				fprintf(fout,"invokevirtual java/util/Scanner/");
				if(node1->type->type==INTEGER_t){fprintf(fout,"nextInt()I\n");}
				else if(node1->type->type==REAL_t){fprintf(fout,"nextFloat()F\n");}
				else if(node1->type->type==BOOLEAN_t){fprintf(fout,"nextBOOLEAN()Z\n");}
				
				if(node1->scope==0){//global
					fprintf(fout,"putstatic %s/%s",fileName,$3 -> varRef -> id);					
				}else{
					if(node1->type->type==INTEGER_t||node1->type->type==BOOLEAN_t){fprintf(fout, "istore %d\n",node1->count);}
					else if(node1->type->type==REAL_t){fprintf(fout, "fstore %d\n",node1->count);}
				}
			}
			;

proc_call_stmt		: ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON
			{
			  verifyFuncInvoke( $1, $3, symbolTable, scope );
			}
			;

cond_stmt		: IF condition 
				THEN
			  opt_stmt_list
			  ELSE{fprintf(fout,"	goto L_%d\n",stackLabel-1);
			  fprintf(fout,"L_%d:\n",stackLabel-2);
			  }
			  opt_stmt_list
			  END IF{fprintf(fout,"L_%d:\n",stackLabel-1);}
			| IF condition THEN
			opt_stmt_list END IF{fprintf(fout,"L_%d:\n",stackLabel-2);}
			;

condition		: boolean_expr { verifyBooleanExpr( $1, "if" );
				fprintf(fout,"	ifeq L_%d\n",stackLabel-2); } 
			;

while_stmt		: WHILE condition_while DO
			  opt_stmt_list
			  END DO
			;

condition_while		: boolean_expr { verifyBooleanExpr( $1, "while" ); } 
			;

for_stmt		: FOR ID 
			{ 
			  insertLoopVarIntoTable( symbolTable, $2 );
			}
			  OP_ASSIGN loop_param TO loop_param
			{
			  verifyLoopParam( $5, $7 );
			}
			  DO
			  opt_stmt_list
			  END DO
			{
			  popLoopVar( symbolTable );
			}
			;

loop_param		: INT_CONST { $$ = $1; }
			| OP_SUB INT_CONST { $$ = -$2; }
			;

return_stmt		: RETURN boolean_expr MK_SEMICOLON
			{
			  verifyReturnStatement( $2, funcReturn );
			  
			}
			;

opt_boolean_expr_list	: boolean_expr_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }	// null
			;

boolean_expr_list	: boolean_expr_list MK_COMMA boolean_expr
			{
			  struct expr_sem *exprPtr;
			  for( exprPtr=$1 ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  exprPtr->next = $3;
			  $$ = $1;
			}
			| boolean_expr
			{
			  $$ = $1;
			}
			;

boolean_expr		: boolean_expr OP_OR boolean_term
			{
			  verifyAndOrOp( $1, OR_t, $3 );
			  $$ = $1;
			  fprintf(fout,"	ior\n");
			}
			| boolean_term { $$ = $1; }
			;

boolean_term		: boolean_term OP_AND boolean_factor
			{
			  verifyAndOrOp( $1, AND_t, $3 );
			  $$ = $1;
			  fprintf(fout,"	iand\n");
			}
			| boolean_factor { $$ = $1; }
			;

boolean_factor		: OP_NOT boolean_factor 
			{
			  verifyUnaryNOT( $2 );
			  $$ = $2;
			  fprintf(fout,"	ixor\n");
			}
			| relop_expr { $$ = $1; }
			;

relop_expr		: expr rel_op expr
			{
				if($1->pType->type==INTEGER_t){//to float
					fprintf(fout,"i2f\n");
				}
				if($3->pType->type==INTEGER_t){//to float
					fprintf(fout,"i2f\n");
				}
				
				fprintf(fout,"fcmpl\n");
				
				if($2==LT_t){
					fprintf( fout, "	iflt L%d\n", stackLabel );
				}else if($2==LE_t){
					fprintf( fout, "	ifle L%d\n", stackLabel );
				}else if($2==EQ_t){
					fprintf( fout, "	ifeq L%d\n", stackLabel );
				}else if($2==GE_t){
					fprintf( fout, "	ifge L%d\n", stackLabel );
				}else if($2==GT_t){
					fprintf( fout, "	ifgt L%d\n", stackLabel );
				}else if($2==NE_t){
					fprintf( fout, "	ifne L%d\n", stackLabel );
				}
				
				fprintf( fout, "	iconst_0\n" );
				fprintf(fout,"	goto L%d\n", stackLabel+1);
				fprintf(fout,"	L%d:\n", stackLabel);
				fprintf( fout, "	iconst_1\n" );
				fprintf(fout,"	L%d:\n", stackLabel+1);
				stackLabel+=2;
				
			    verifyRelOp( $1, $2, $3 );
			  $$ = $1;
			}
			| expr { $$ = $1; }
			;

rel_op			: OP_LT { $$ = LT_t; }
			| OP_LE { $$ = LE_t; }
			| OP_EQ { $$ = EQ_t; }
			| OP_GE { $$ = GE_t; }
			| OP_GT { $$ = GT_t; }
			| OP_NE { $$ = NE_t; }
			;

expr			: expr
			{
				if($1->pType->type==INTEGER_t){
					fprintf( fout, "	i2f\n");
				}
			}
			add_op term
			{ 
				
			 if($4->pType->type==INTEGER_t){
					fprintf( fout, "	i2f\n");
				}
			 
			  verifyArithmeticOp( $1, $3, $4 );
			  $$ = $1;
			  
				if($3==ADD_t){fprintf( fout, "	fadd\n" );}
				else if($3==SUB_t){fprintf( fout, "	fsub\n" );}
				
				if($4->pType->type==INTEGER_t&&$1->pType->type==INTEGER_t)
					fprintf( fout, "	f2i\n");
			  
			}
			| term { $$ = $1; }
			;

add_op			: OP_ADD { $$ = ADD_t; }
			| OP_SUB { $$ = SUB_t; }
			;

term			: term mul_op factor
			{	
				
			if($3->pType->type==INTEGER_t&&$1->pType->type==REAL_t){
					fprintf( fout, "	i2f\n");
				}
			 else if($1->pType->type==INTEGER_t&&$3->pType->type==REAL_t){
					
					//fprintf( fout, "	fstore %d\n", n1->count);
					fprintf( fout, "	i2f\n");
					//fprintf( fout, "	fload %d\n", n2->count);
				}
				if($2==MOD_t){verifyModOp( $1, $3 );}
			  else{verifyArithmeticOp( $1, $2, $3 );}
			  $$ = $1;
				if($1->pType->type==INTEGER_t&&$1->pType->type==INTEGER_t){fprintf(fout,"i");}
				else{fprintf(fout,"f");}
				if($2==MUL_t){fprintf( fout, "mul\n" );}
				else if($2==DIV_t){fprintf( fout, "div\n" );}
				else if($2==MOD_t){fprintf( fout, "rem\n" );}
				
			}
			| factor { 
				
				$$ = $1; 
				if($$->beginningOp==SUB_t){
					if($1->pType->type==INTEGER_t){
						fprintf( fout, "	ineg\n" );
					}else{
						fprintf( fout, "	fneg\n" );
					}
				}
			}
			;

mul_op			: OP_MUL { $$ = MUL_t; }
			| OP_DIV { $$ = DIV_t; }
			| OP_MOD { $$ = MOD_t; }
			;

factor			: var_ref
			{	
				verifyExistence( symbolTable, $1, scope, __FALSE );
				$$ = $1;
				$$->beginningOp = NONE_t;
				struct SymNode *node1 = lookupSymbol( symbolTable, $1 -> varRef -> id, scope, __FALSE );
				if(node1->category==CONSTANT_t){//constant
					if(node1->type->type==INTEGER_t){//int
						fprintf(fout,"	sipush %d\n",node1->attribute->constVal->value.integerVal);
					}else if(node1->type->type==BOOLEAN_t){//true false
						if(node1->attribute->constVal->value.booleanVal==TRUE){fprintf(fout,"iconst_1\n");}
						else if(node1->attribute->constVal->value.booleanVal==FALSE){fprintf(fout,"iconst_0\n");}
					}else if(node1->type->type==REAL_t){//real
						fprintf(fout,"ldc %f\n",node1->attribute->constVal->value.realVal);
					}else if(node1->type->type==STRING_t){//string
						fprintf(fout,"ldc \"%s\"\n",node1->attribute->constVal->value.stringVal);
					}
				}else if(node1->scope==0){//global
					fprintf(fout,"getstatic %s/%s ",fileName,node1->name);
					if(node1->type->type==INTEGER_t){fprintf(fout,"I\n");}
					else if(node1->type->type==BOOLEAN_t){fprintf(fout,"Z\n");}
					else if(node1->type->type==REAL_t){fprintf(fout,"F\n");}
				}else if(node1->scope>0){//local
					if(node1->type->type==INTEGER_t){fprintf(fout,"	iload %d\n",node1->count);}
					else if(node1->type->type==BOOLEAN_t){fprintf(fout,"	iload %d\n",node1->count);}
					else if(node1->type->type==REAL_t){fprintf(fout,"	fload %d\n",node1->count);}
					
				}
				
			  
			}
			| OP_SUB var_ref
			{	
			if(r==1){
				if( verifyExistence( symbolTable, $2, scope, __TRUE ) == __TRUE ){
					verifyUnaryMinus($2);
					
				}
				r=0;
			}else{
				if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE ){
					verifyUnaryMinus($2);
				}
			}
			  //if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE )
				//verifyUnaryMinus( $2 );
			  $$ = $2;
			  $$->beginningOp = SUB_t;
			}
			| MK_LPAREN boolean_expr MK_RPAREN 
			{
			  $2->beginningOp = NONE_t;
			  $$ = $2; 
			}
			| OP_SUB MK_LPAREN boolean_expr MK_RPAREN
			{
			  verifyUnaryMinus( $3 );
			  $$ = $3;
			  $$->beginningOp = SUB_t;
			}
			| ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{	
				fprintf(fout,"	invokestatic %s/%s(",fileName,$1);
				struct expr_sem *p;
				for(p=$3;p!=NULL;p=p->next){fprintf(fout,"%c",decl_type(p->pType));}
				struct SymNode *node=lookupSymbol(symbolTable,$1,scope,__FALSE);
				fprintf(fout,")%c\n",decl_type(node->type));
			  $$ = verifyFuncInvoke( $1, $3, symbolTable, scope );
			  $$->beginningOp = NONE_t;
			}
			| OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{
				fprintf(fout,"	invokestatic %s/%s(",fileName,$2);
				struct expr_sem *p;
				for(p=$4;p!=NULL;p=p->next){fprintf(fout,"%c",decl_type(p->pType));}
				struct SymNode *node=lookupSymbol(symbolTable,$2,scope,__FALSE);
				fprintf(fout,")%c\n",decl_type(node->type));
			  $$ = verifyFuncInvoke( $2, $4, symbolTable, scope );
			  $$->beginningOp = SUB_t;
			}
			| literal_const
			{
			  $$ = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  $$->isDeref = __TRUE;
			  $$->varRef = 0;
			  $$->pType = createPType( $1->category );
			  $$->next = 0;
			  if( $1->hasMinus == __TRUE ) {
			  	$$->beginningOp = SUB_t;
			  }
			  else {
				$$->beginningOp = NONE_t;
			  }
			  
			  if($$->pType->type==INTEGER_t){
				fprintf(fout,"	ldc %d\n",$1->value.integerVal);
			  }else if($$->pType->type==BOOLEAN_t){
				fprintf(fout,"	iconst_%d\n",$1->value.booleanVal);
			  }else if($$->pType->type==STRING_t){
				fprintf(fout,"	ldc \"%s\"\n",$1->value.stringVal);
			  }else if($$->pType->type==REAL_t){
				fprintf(fout,"	ldc %f\n",$1->value.realVal);
			}
			}
			;

var_ref			: ID
			{
			  
			  $$ = createExprSem( $1 );
			  
			}
			| var_ref dim
			{
			  increaseDim( $1, $2 );
			  $$ = $1;
			}
			;

dim			: MK_LB boolean_expr MK_RB
			{
			  $$ = verifyArrayIndex( $2 );
			}
			;

%%

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

