%{
/**
 * Introduction to Compiler Design by Prof. Yi Ping You
 * Project 2 YACC sample
 */
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

extern int linenum;		/* declared in lex.l */
extern FILE *yyin;		/* declared by lex */
extern char *yytext;		/* declared by lex */
extern char buf[256];		/* declared in lex.l */
extern int Opt_D;
//extern int yylex(void);
//int yyerror(char* );
int level=0;
char pro_name[256];
int kind=0;//0:program,1:function,2:parameter,3:variable,4:contest
int type=0;//0:int,1:real,2:bool,3:string,4:array,5:void
int id_list_i=4;
int a_n1,a_n2;
char name[256];
struct var_name{
	char name[256];
	struct var_name *next;
}v_name[256];
struct attribute{
	int a;
	char inf[256];
}attr;
struct array{
	int size;
	int type;
	struct array *next;
}arr;
int isarray=0;//0:no 1:yes
int isfunc=0;//0:no 1:yes
struct function {
	int type;
	int isarray;
	struct function* next;
}func;
struct table_list{
	char name[256];
	int kind;
	int type;
	struct attribute attr;
	int cur_level;
	int isarray;
	struct array arr;
	struct table_list *next;
	int isfunc;
	struct function func
}list[256];
void create_list(char name[],int kind,int level,int type,int isarray,struct array arr,struct attribute attr,int isfunc){
	struct table_list *t=(struct table_list*) malloc (sizeof(struct table_list));
	strcpy(t->name,name);//t->name=name;
	t->kind=kind;
	t->type=type;
	t->attr=attr;
	t->cur_level=level;
	t->isarray=0;
//	t.arr=arr;
	array_list1(t->arr,arr);
	t->next=NULL;
	t->isfunc=0;
//	t.func=func;
	struct table_list *tem=list[level].next;
	while(tem->next!=NULL){
		if(strcmp(tem->name,name)==0){
			printf("\n|--------------------------------------------------------------------------\n");
			printf( "| Error found in Line #%d: variable redeclaration: %s\n", linenum,"variable redecl",name );
			printf( "|--------------------------------------------------------------------------\n" );
			return;
		}else{tem=tem->next;}
		
	}
	tem->next=t;
}
void create_array(int size){//a=b
	struct array *t=arr.next;
	while(t->next!=NULL){t=t->next;}
	struct array *n=(struct array*) malloc(sizeof(struct array));
	n->size=size;
	n->next=NULL;
	t->next=n;
}
void array_list1(struct array *a,struct array *b){//a=b
	struct array *t=b->next;
	struct array *tem=a->next;
	struct array *n=(struct array*) malloc (sizeof(struct array));
	int count=0;
	while(t!=NULL){
		n->size=t->size;
		n->next=NULL;
		if(count==0){b->next=n;count++;}
		else{tem->next=n;count++;}
		t=t->next;
		tem=tem->next;
	}
}
void dumpsymbol();
%}
/* tokens */
%token ARRAY
%token BEG
%token BOOLEAN
%token DEF
%token DO
%token ELSE
%token END
%token FALSE
%token FOR
%token INTEGER
%token IF
%token OF
%token PRINT
%token READ
%token REAL
%token RETURN
%token STRING
%token THEN
%token TO
%token TRUE
%token VAR
%token WHILE

%token ID
%token OCTAL_CONST
%token INT_CONST
%token FLOAT_CONST
%token SCIENTIFIC
%token STR_CONST

%token OP_ADD
%token OP_SUB
%token OP_MUL
%token OP_DIV
%token OP_MOD
%token OP_ASSIGN
%token OP_EQ
%token OP_NE
%token OP_GT
%token OP_LT
%token OP_GE
%token OP_LE
%token OP_AND
%token OP_OR
%token OP_NOT

%token MK_COMMA
%token MK_COLON
%token MK_SEMICOLON
%token MK_LPAREN
%token MK_RPAREN
%token MK_LB
%token MK_RB

/* start symbol */
%start program
%%

program			: ID MK_SEMICOLON {name[0]='\0';
									strcpy(name,buf);
									int q=strlen(name);
									name[q-1]='\0';
									kind=0;level=0;type=5;
									isarray=0;isfunc=0;
									list[level].next=NULL;
									arr.next=NULL;
									attr.inf[0]='\0';
									func.next=NULL;
									isfunc=0;
									//create_list(name,kind,level,type,isarray,arr,attr,isfunc)
									create_list(name,0,0,type,isarray,arr,attr,isfunc);
								}
			  program_body
			  END ID {name[0]='\0';
					strcpy(name,buf);
					int q=strlen(name);
					int i=0;
					for(i=0;i<q-4;i++){name[i]=name[i+4];}
					name[i]='\0';
					if(strcmp(name,list[level].next->name)==0){dumpsymbol();}
					else{printf("error\n");}
					dumpsymbol();}
			;

program_body		: opt_decl_list opt_func_decl_list compound_stmt{dumpsymbol();level=level-1;/*leave level*/}
			;

opt_decl_list		: decl_list
			| /* epsilon */
			;

decl_list		: decl_list decl
			| decl
			;

decl			: VAR id_list{id_list_i=4;} MK_COLON scalar_type MK_SEMICOLON       /* scalar type declaration */
				{
					isarray=0;
					struct table_list *t=list[level].next;
					struct var_name* n=v_name[level].next;
					while(n!=NULL){
						create_list(n->name,kind,level,type,isarray,arr,attr,isfunc);
						n=n->next;
					}
					v_name[level].next=NULL;
				}
			| VAR id_list{id_list_i=4;} MK_COLON array_type MK_SEMICOLON        /* array type declaration */
			{
				isarray=1;
				struct table_list *t=list[level].next;
				struct var_name* n=v_name[level].next;
				while(n!=NULL){
					create_list(n->name,kind,level,type,isarray,arr,attr,isfunc);
					n=n->next;
				}
				v_name[level].next=NULL;
				arr.size=0;
				arr.next=NULL;
			}
			| VAR id_list {id_list_i=4;}MK_COLON literal_const MK_SEMICOLON     /* const declaration */
			{
				isarray=0;
				struct table_list *t=list[level].next;
				struct var_name* n=v_name[level].next;
				while(n!=NULL){
					create_list(n->name,kind,level,type,isarray,arr,attr,isfunc);
					n=n->next;
				}
				v_name[level].next=NULL;
			}
			;
int_const	:	INT_CONST{int k,l,i,j=0;
						l=strlen(buf);
						k=l-1;
						while(k>=0){if(buf[k]!=' '){break;}else{k--;}}
						j=atoi(buf[k]);
						for(i=k+1;i<l;i++){j=10*j+atoi(buf[i]);}
						attr.a=j;
					}
			|	OCTAL_CONST{
						int k,l,i,j=0;
						l=strlen(buf);
						k=l-1;
						while(k>=0){if(buf[k]!=' '){break;}else{k--;}}
						j=atoi(buf[k]);
						for(i=k+1;i<l;i++){j=10*j+atoi(buf[i]);}
						attr.a=j;
			
			}
			;

literal_const		: int_const{type=0;}
/**/			| OP_SUB int_const{type=0;attr.a=attr.a*(-1);}
			| FLOAT_CONST{type=6;}
			| OP_SUB FLOAT_CONST{type=6;}
			| SCIENTIFIC{type=6;}
			| OP_SUB SCIENTIFIC{type=6;}
			| STR_CONST{type=3;}
			| TRUE{type=2;}
			| FALSE{type=2;}
			;

opt_func_decl_list	: func_decl_list
			| /* epsilon */
			;

func_decl_list		: func_decl_list func_decl
			| func_decl
			;

func_decl		: ID{level=level+1;} MK_LPAREN opt_param_list MK_RPAREN opt_type 
					MK_SEMICOLON{
						level=level-1;
						int i;
						for(i=0;i<strlen(buf);i++){if(buf[i]!='('){name[i]=buf[i];}else{break;}}
						kind=1;
						create_list(name,kind,level,type,0,arr,attr,1);
					}
/**/			  compound_stmt
			  END ID{dumpsymbol();level=level-1;}
			;

opt_param_list		: param_list
			| /* epsilon */
			;

param_list		: param_list MK_SEMICOLON param
			| param
			;

param			: id_list{id_list_i=4;isarray=0;} MK_COLON type{
					struct var_name *t=v_name[level].next;
					while(t!=NULL){
						create_list(t->name,kind,level,type,0,arr,attr,1);
						t=t->next;
					}
					arr.size=0;
					arr.next=NULL;
					v_name[level].name[0]='\0';
					v_name[level].next=NULL;
				}
/**//**/			;

id_list			: id_list MK_COMMA ID{
					name[0]='\0';
					struct var_name* t=v_name[level].next;
					int j=0;
					struct var_name *tem=(struct var_name*) malloc (sizeof(struct var_name));
					while(id_list_i<strlen(buf)){
						if(buf[id_list_i]!=','&&buf[id_list_i]!=' '){name[j]=buf[id_list_i];id_list_i++;j++;}
						else if(buf[id_list_i]==' '){break;}
					}
					strcpy(tem->name,name);
					//tem->name=name;
					tem->next=NULL;
					while(t->next!=NULL){t=t->next;}
					t->next=tem;
				}
			| ID{
				name[0]='\0';
				int i,j=0;
				for(i=4;i<strlen(buf);i++){
					if(buf[i]!=' '){name[j]=buf[i];j++;}
				}
				struct var_name* t=v_name[level].next;
				struct var_name *tem=(struct var_name*) malloc (sizeof(struct var_name));
				strcpy(tem->name,name);
				//tem->name=name;
				tem->next=NULL;
				while(t->next!=NULL){t=t->next;}
				t->next=tem;
			}
			;

opt_type		: MK_COLON type
/**/			| /* epsilon */
			;

type			: scalar_type
			| array_type{type=4;}
			;

scalar_type		: INTEGER{type=0;}
			| REAL{type=1;}
			| BOOLEAN{type=2;}
			| STRING{type=3;}
			;

array_type		: ARRAY{isarray=1;} int_const{a_n1=attr.a;} TO int_const{a_n2=attr.a;
																			if(a_n2<=a_n1){yyerror("array declaration error");}
																			else{create_array(1+a_n2-a_n1);}} OF type
			;

stmt			: compound_stmt{dumpsymbol();level--;}
			| simple_stmt
			| cond_stmt
			| while_stmt
			| for_stmt
			| return_stmt
			| proc_call_stmt
			;

compound_stmt		: BEG{level++;}
			  opt_decl_list
			  opt_stmt_list
			  END
			;

opt_stmt_list		: stmt_list
			| /* epsilon */
			;

stmt_list		: stmt_list stmt
			| stmt
			;

simple_stmt		: var_ref OP_ASSIGN boolean_expr MK_SEMICOLON
/**/			| PRINT boolean_expr MK_SEMICOLON
			| READ boolean_expr MK_SEMICOLON
			;

proc_call_stmt		: ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON
/**/			;

cond_stmt		: IF boolean_expr THEN
			  opt_stmt_list
			  ELSE
			  opt_stmt_list
			  END IF
			| IF boolean_expr THEN opt_stmt_list END IF
			;

while_stmt		: WHILE boolean_expr DO
			  opt_stmt_list
			  END DO
			;

for_stmt		: FOR ID OP_ASSIGN int_const{a_n1=attr.a;}TO int_const{a_n2=attr.a;} DO
			  opt_stmt_list
			  END DO
			;

return_stmt		: RETURN boolean_expr MK_SEMICOLON{if(kind!=1){yyerror("return error");}}
/**/			;

opt_boolean_expr_list	: boolean_expr_list
			| /* epsilon */
			;

boolean_expr_list	: boolean_expr_list MK_COMMA boolean_expr
/**/			| boolean_expr
			;

boolean_expr		: boolean_expr{if(type!=2){yyerror("not the boolean type\n");}} OP_OR boolean_term{if(type!=2){yyerror("not the boolean type\n");}}
/**/			| boolean_term
			;

boolean_term		: boolean_term{if(type!=2){yyerror("not the boolean type\n");}} OP_AND boolean_factor{if(type!=2){yyerror("not the boolean type\n");}}
/**/			| boolean_factor
			;

boolean_factor		: OP_NOT boolean_factor {if(type!=2){yyerror("not the boolean type\n");}}
/**/			| relop_expr
			;

relop_expr		: expr rel_op expr{if(type!=0&&type!=6){yyerror("not the int or real type\n");}}
/**/			| expr{if(type!=0&&type!=6){yyerror("not the int or real type\n");}}
			;

rel_op			: OP_LT
			| OP_LE
			| OP_EQ
			| OP_GE
			| OP_GT
			| OP_NE
			;

expr			: expr{if(type!=0&&type!=6&&type!=3){yyerror("not the int or real type\n");}} add_op term{if(type!=0&&type!=6&&type!=3){yyerror("not the int or real type\n");}}
/**/			| term
			;

add_op			: OP_ADD
/**/			| OP_SUB
			;

term			: term {if(type!=0&&type!=6&&type!=3){yyerror("not the int or real type\n");}}mul_op factor {if(type!=0&&type!=6&&type!=3){yyerror("not the int or real type\n");}}
/**/			| factor
			;

mul_op			: OP_MUL
/**/			| OP_DIV
			| OP_MOD
			;

factor			: var_ref
/**/			| OP_SUB var_ref
			| MK_LPAREN boolean_expr MK_RPAREN
			| OP_SUB MK_LPAREN boolean_expr MK_RPAREN
			| ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			| OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			| literal_const
			;

var_ref			: ID
/**/			| var_ref dim
			;

dim			: MK_LB boolean_expr MK_RB
/**/			;

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

void dumpsymbol(){//
	if(Opt_D==1){
		int i;
		printf("%-32s\t%-11s\t%-11s\t%-17s\t%-11s\t\n","Name","Kind","Level","Type","Attribute");
		for(i=0;i<110;i++){printf("-");}
		printf("\n");
		struct table_list *t=list[level].next;
		while(t!=NULL){
			printf("%-32s\t", t->name);
			
			if(t->kind==0){printf("%-11s\t", "program");}
			else if(t->kind==1){printf("%-11s\t", "function");}
			else if(t->kind==2){printf("%-11s\t", "parameter");}
			else if(t->kind==3){printf("%-11s\t", "variable");}
			else if(t->kind==4){printf("%-11s\t", "contest");}
			
			if(t->cur_level==0){printf("%d%-10s\t",t->cur_level,"(global)");}
			else{printf("%d%-10s\t",t->cur_level,"(local)");}
			
			if(t->type==0){printf("%-17s\t", "integer");}
			else if(t->type==1){printf("%-17s\t", "real");}
			else if(t->type==2){printf("%-17s\t", "boolean");}
			else if(t->type==3){printf("%-17s\t", "string");}
			else if(t->type==4){printf("%-17s\t", "array");}
			else if(t->type==5){printf("%-17s\t", "void");}
			else if(t->type==6){printf("%-17s\t", "float");}
			
			t=t->next;
		}
	}
}

