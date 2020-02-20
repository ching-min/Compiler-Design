%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%start program;
%token SEMICOLON END IDENT COLON COMMA
%token L_BEGIN ARRAY BOOLEAN DEF DO ELSE FALSE FOR INT IF OF  
%token PRINT READ REAL STRING THEN TO TRUE RETURN VAR WHILE
%left '+' '-'
%left '*' '/' MOD
%right ASSIGNMENT
%nonassoc '>' '<' LESS_EQ NOT_EQ MORE_EQ AND OR NOT '='
%token SCIENTIFIC NUM FLOAT OCT_INTEGER ID STR 


%%

program		: programname SEMICOLON programbody END IDENT
		;

programname	: identifier
		;

identifier	: IDENT
		;

programbody	:	vcdecl function_decl compound	/*variable decl&const decl, function decl,compound*/ 
		;
vcdecl	:
		|variable_decl SEMICOLON vcdecl
		|const_decl SEMICOLON vcdecl
		;
variable_decl:
			|VAR identifier_list COLON scalar_type 
			|VAR identifier_list COLON array_type OF scalar_type 
			|VAR identifier_list COLON array_type OF array_type OF scalar_type 			
			;
scalar_type:INT
			|BOOLEAN
			|REAL
			|STRING
			;
array_type:ARRAY int_p TO int_p;
int_p: NUM;
literal_const:NUM
			|STR
			|TRUE
			|FALSE
			;
integer_literal:SCIENTIFIC
				|NUM
				|FLOAT
				|OCT_INTEGER
				;
identifier_list:/*zero*/
				|identifier
				|identifier COMMA identifier_list
				;
const_decl: /*zero*/
			|VAR identifier_list COLON literal_const
			|VAR identifier_list COLON integer_literal
			
		;
function_decl:/*zero*/
			|identifier '(' argument_list ')' SEMICOLON compound END identifier function_decl
			|identifier '(' argument_list ')' COLON scalar_type SEMICOLON compound END identifier function_decl
			|identifier '(' argument_list ')' COLON array_type SEMICOLON compound END identifier function_decl
			;

/**/statement:/*zero*/
		|compound statement
		|simple statement
		|conditional statement
		|while statement
		|for statement
		|return statement
		|function_invocation statement
		|identifier_list ASSIGNMENT literal_const SEMICOLON statement
		|identifier_list ASSIGNMENT expression SEMICOLON statement		
		;
st_decl:/*zero*/
		|VAR identifier_list COLON scalar_type SEMICOLON st_decl
		|VAR identifier_list COLON array_type OF scalar_type SEMICOLON st_decl
		|VAR identifier_list COLON array_type OF array_type OF scalar_type SEMICOLON st_decl
		|VAR identifier_list COLON literal_const SEMICOLON st_decl
		|ARRAY int_p TO int_p SEMICOLON st_decl
		;
compound:/*zero*/
		|L_BEGIN st_decl statement END compound
		|L_BEGIN statement END compound
		;
argument_list:/*zero*/
			|literal_const
			|integer_exp COMMA argument_list
			|expression COMMA argument_list
			|identifier COLON scalar_type COMMA argument_list
			|identifier COLON scalar_type
			|identifier COLON array_type COMMA argument_list
			|identifier			
			;
simple:/*zero*/
		|variable_ref ASSIGNMENT expression SEMICOLON simple
		|PRINT variable_ref SEMICOLON simple
		|PRINT expression SEMICOLON simple
		|PRINT STR SEMICOLON simple
		|PRINT identifier '(' argument_list ')' SEMICOLON simple
		|READ variable_ref SEMICOLON simple
	;
variable_ref:identifier
			|array_ref
			;
array_ref: identifier '[' integer_exp ']' array_ext;
array_ext:/*zero*/
			|'[' integer_exp ']' array_ext;
integer_exp: identifier operator integer_literal
				|integer_literal 
				|integer_literal operator integer_exp
				|'(' integer_exp ')'
				|integer_literal
				|identifier operator integer_exp
				|identifier
				;
expression: integer_literal
			|integer_con
			|boolean_exp
			|integer_exp;
operator:'-'
		|'*'
		|'/'
		|MOD
		|'+'
		|'-'
		;
con_operator_1:'<'
				|LESS_EQ
				|'='
				|'>'
				|MORE_EQ
				|NOT_EQ
				;
con_operator_2:|NOT
				|AND
				|OR
				;
expression_list:
				|expression
				|expression COMMA expression_list;
boolean_exp:identifier con_operator_1 identifier
			|identifier con_operator_1 NUM
			|integer_exp con_operator_1 integer_exp
			|boolean_exp con_operator_2 boolean_exp
			;
conditional:IF boolean_exp THEN statement ELSE statement END IF
			|IF boolean_exp THEN statement END IF
			;
while: WHILE boolean_exp DO statement END DO;
for:FOR identifier ASSIGNMENT integer_con TO integer_con DO statement END DO;
return: RETURN expression SEMICOLON;
integer_con:NUM;
function_invocation:identifier '(' expression_list ')' SEMICOLON;

		;
%%

int yyerror( char *msg )
{
        fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
        fprintf( stderr, "|--------------------------------------------------------------------------\n" );
        exit(-1);
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}
