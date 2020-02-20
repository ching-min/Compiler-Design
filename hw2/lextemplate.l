%{
#include "y.tab.h"
#define LIST               strcat(buf, yytext)
#define token(t)           {LIST; if (Opt_T) printf("<%s>\n", #t);}
#define tokenChar(t)       {LIST; if (Opt_T) printf("<%c>\n", (t));}
#define tokenInteger(t, i) {LIST; if (Opt_T) printf("<%s: %d>\n", #t, (i));}
#define tokenString(t, s)  {LIST; if (Opt_T) printf("<%s: %s>\n", #t, (s));}
#define tokenoctInteger(t, i) {LIST; if (Opt_T) printf("<%s: 0%o>\n", #t, (i));}
#define MAX_LINE_LENG      256

int Opt_S = 1;
int Opt_T = 1;
int linenum = 1;
int flag=1;
int com_f=0;
char buf[MAX_LINE_LENG];
%}
%Start COMMENT
%Start COMMENT1
%%
<COMMENT>"*"	{LIST;
				int i=0;
				yytext[0]='\0';
				BEGIN COMMENT1;
				}
<COMMENT>[^"*"]*	{LIST;
				int i=0;
				yytext[0]='\0';
				BEGIN COMMENT;
				}
<COMMENT1>"/" {LIST;Opt_T=1;BEGIN INITIAL; }
<COMMENT1>[^"/"] {LIST;BEGIN COMMENT;}

"/*" {LIST;Opt_T = 0;com_f=1;BEGIN COMMENT;}
[ \t] {LIST;}

"//&S+"[^\n]*	{	Opt_S = 1;
					yytext[5]='\0';
					LIST;
				}
"//&S-"[^\n]*	{	Opt_S = 0;
					yytext[5]='\0';
					LIST;
				}
"//&T+"[^\n]*	{	Opt_T = 1;
					yytext[5]='\0';
					LIST;
				}
"//&T-"[^\n]*   {	Opt_T = 0;
					yytext[5]='\0';
					LIST;
				}
"//"[^\n]*	{LIST;}
","		{ tokenChar(',');return COMMA;}
";"		{ tokenChar(';');return SEMICOLON;}	
"("     { tokenChar('('); return '(';}
"["		{ tokenChar('[');return '[';}
"]"		{ tokenChar(']');return ']';}
":"		{ tokenChar(':');return COLON;}
")"		{ tokenChar(')');return ')';}	

\n      {
          LIST;
		  int i=0;
          if (Opt_S){
			if(com_f==0){printf("%d: %s", linenum, buf);}
			else{
				printf("%d: ", linenum);
				linenum++;
				for(;i<strlen(buf)-1;i++){
					if(buf[i]!='\n'){printf("%c",buf[i]);}
					else{printf("%c%d: ",buf[i],linenum);linenum++;}
				}
				printf("%c",buf[i]);
				linenum--;
				com_f=0;
			}
			
		  }
            
          linenum++;
          if(flag==1){buf[0] = '\0';}
		  
        }

(begin)	{LIST;if(Opt_T==1){ printf("<KWbegin>\n");} return L_BEGIN;}
(array) {LIST;if(Opt_T==1){ printf("<KWarray>\n");} return ARRAY;}
(boolean) {LIST;if(Opt_T==1){printf("<KWboolean>\n");} return BOOLEAN;}
(def) {LIST;if(Opt_T==1){printf("<KWdef>\n");} return DEF;}
(do) {LIST; if(Opt_T==1){printf("<KWdo>\n");}return DO;}
(else) {LIST; if(Opt_T==1){printf("<KWelse>\n");} return ELSE;}
(end)	{LIST; if(Opt_T==1){printf("<KWend>\n");} return END;}
(false)	{LIST; if(Opt_T==1){printf("<KWfalse>\n");} return FALSE;}
(for)	{LIST; if(Opt_T==1){printf("<KWfor>\n");} return FOR;}
(integer)	{LIST; if(Opt_T==1){printf("<KWinteger>\n");} return INT;}
(if)	{LIST; if(Opt_T==1){printf("<KWif>\n");} return IF;}
(of)	{LIST; if(Opt_T==1){printf("<KWof>\n");} return OF;}
(print)	{LIST; if(Opt_T==1){printf("<KWprint>\n");} return PRINT;}
(read)	{LIST; if(Opt_T==1){printf("<KWread>\n");} return READ;}
(real)	{LIST; if(Opt_T==1){printf("<KWreal>\n");} return REAL;}
(string)	{LIST; if(Opt_T==1){printf("<KWstring>\n");} return STRING;}
(then)	{LIST; if(Opt_T==1){printf("<KWthen>\n");} return THEN;}
(to) {LIST; if(Opt_T==1){printf("<KWto>\n");} return TO;}
(true)	{LIST; if(Opt_T==1){printf("<KWtrue>\n");} return TRUE;}
(return) {LIST; if(Opt_T==1){printf("<KWreturn>\n");} return RETURN;}
(var) {LIST; if(Opt_T==1){printf("<KWvar>\n");} return VAR;} 
(while) {LIST;if(Opt_T==1){printf("<KWwhile>\n");} return WHILE;} 

"+"		{ tokenChar('+'); return '+';}
"-"		{ tokenChar('-'); return '-';}
"*"		{ tokenChar('*'); return '*';}
"/"		{ tokenChar('/'); return '/';}
"<"		{ tokenChar('<'); return '<';}
">"		{ tokenChar('>'); return '>';}
"="		{ tokenChar('='); return '=';}
":="		{LIST;if(Opt_T==1){printf("<:=>\n");} return ASSIGNMENT;}
"<="		{LIST;if(Opt_T==1){printf("<<=>\n");} return LESS_EQ;}
"<>"		{LIST;if(Opt_T==1){printf("<<>>\n");} return NOT_EQ;}
">="		{LIST;if(Opt_T==1){printf("<>=>\n");} return MORE_EQ;}
"mod"		{LIST;if(Opt_T==1){printf("<mod>\n");} return MOD;}
"and"		{LIST;if(Opt_T==1){printf("<and>\n");} return AND;}
"or"		{LIST;if(Opt_T==1){printf("<or>\n");} return OR;}
"not"		{LIST;if(Opt_T==1){printf("<not>\n");} return NOT;}

(0|[1-9][0-9]*)(\.)?([0-9])*[Ee][+-](0|[1-9][0-9]*) { tokenString(scientific,yytext);return SCIENTIFIC;}
0|[1-9][0-9]* {tokenInteger(integer,atoi(yytext));return NUM;}
(0|[1-9][0-9]*)\.[0-9]+ {tokenString(float,yytext);return FLOAT;}
0(0|[1-7]*)	{ tokenoctInteger(oct_integer, atoi(yytext));return OCT_INTEGER;}

([A-Za-z])([A-Za-z0-9])* {tokenString(id,yytext);return IDENT;}


\"[^\"]*\"\"[^\"]*\"	{
	int flag=0,i=1,j=0,k=0;
	char yy_tt[strlen(yytext)];
	for(;j<strlen(yytext);j++){yy_tt[j]=yytext[j];}
	for(;i<strlen(yytext)-1;i++){
		if(yy_tt[i]=='\"'&&yy_tt[i+1]=='\"'){flag=1;}
		else if(flag==1){yy_tt[i]=yy_tt[i+1];}
	}	
	yy_tt[strlen(yytext)-1]='\0';
	for(;k<strlen(yy_tt)-2;k++){yy_tt[k]=yy_tt[k+1];}
	yy_tt[k]='\0';
	tokenString(string,yy_tt);
	return STR;
}

\"[^\"]*\"	{
	int i=0,j=0;
	char yy_tt[strlen(yytext)];
	for(;j<strlen(yytext);j++){yy_tt[j]=yytext[j];}
	for(;i<strlen(yytext)-2;i++){
		yy_tt[i]=yy_tt[i+1];
	}
	for(;i<strlen(yytext);i++){yy_tt[i]='\0';}
	
	tokenString(string,yy_tt);
	return STR;
}
.				{
				  /* error */
				  printf("error at line %d: bad character \"%s\"\n", linenum, yytext );
				  exit(-1);
				}
%%

