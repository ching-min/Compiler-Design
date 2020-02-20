%{
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

"/*" {LIST;Opt_T = 0;BEGIN COMMENT;}
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
","		{ tokenChar(',');}
";"		{ tokenChar(';');}	
"("     { tokenChar('('); }
"["		{ tokenChar('[');}
"]"		{ tokenChar(']');}
":"		{ tokenChar(':');}
")"		{ tokenChar(')');}	

\n      {
          LIST;
          if (Opt_S)
            printf("%d: %s", linenum, buf);
          linenum++;
		  int i=0;
		  for(;i<strlen(buf)-1;i++){if(buf[i]=='\n'){linenum++;}}
          if(flag==1){buf[0] = '\0';}
		  
        }

(begin)	{LIST;if(Opt_T==1){ printf("<KWbegin>\n");}}
(array) {LIST;if(Opt_T==1){ printf("<KWarray>\n");}}
(boolean) {LIST;if(Opt_T==1){printf("<KWboolean>\n");}}
(def) {LIST;if(Opt_T==1){printf("<KWdef>\n");}}
(do) {LIST; if(Opt_T==1){printf("<KWdo>\n");}}
(else) {LIST; if(Opt_T==1){printf("<KWelse>\n");}}
(end)	{LIST; if(Opt_T==1){printf("<KWend>\n");}}
(false)	{LIST; if(Opt_T==1){printf("<KWfalse>\n");}}
(for)	{LIST; if(Opt_T==1){printf("<KWfor>\n");}}
(integer)	{LIST; if(Opt_T==1){printf("<KWinteger>\n");}}
(if)	{LIST; if(Opt_T==1){printf("<KWif>\n");}}
(of)	{LIST; if(Opt_T==1){printf("<KWof>\n");}}
(print)	{LIST; if(Opt_T==1){printf("<KWprint>\n");}}
(read)	{LIST; if(Opt_T==1){printf("<KWread>\n");}}
(real)	{LIST; if(Opt_T==1){printf("<KWreal>\n");}}
(string)	{LIST; if(Opt_T==1){printf("<KWstring>\n");}}
(then)	{LIST; if(Opt_T==1){printf("<KWthen>\n");}}
(to) {LIST; if(Opt_T==1){printf("<KWto>\n");}}
(true)	{LIST; if(Opt_T==1){printf("<KWtrue>\n");}}
(return) {LIST; if(Opt_T==1){printf("<KWreturn>\n");}}
(var) {LIST; if(Opt_T==1){printf("<KWvar>\n");}} 
(while) {LIST;if(Opt_T==1){printf("<KWwhile>\n");}} 

"+"		{ tokenChar('+');}
"-"		{ tokenChar('-');}
"*"		{ tokenChar('*');}
"/"		{if(Opt_T == 1){ tokenChar('/');}}
"<"		{ tokenChar('<');}
">"		{ tokenChar('>');}
"="		{ tokenChar('=');}
":="		{LIST;printf("<:=>\n");}
"<="		{LIST;printf("<<=>\n");}
"<>"		{LIST;printf("<<>>\n");}
">="		{LIST;printf("<>=>\n");}
"mod"		{LIST;printf("<mod>\n");}
"and"		{LIST;printf("<and>\n");}
"or"		{LIST;printf("<or>\n");}
"not"		{LIST;printf("<not>\n");}

(0|[1-9][0-9]*)(\.)?([0-9])*[Ee][+-](0|[1-9][0-9]*) { tokenString(scientific,yytext);}
0|[1-9][0-9]* {tokenInteger(integer,atoi(yytext));}
(0|[1-9][0-9]*)\.[0-9]* {tokenString(float,yytext);}
0(0|[1-7]*)	{ tokenoctInteger(oct_integer, atoi(yytext));}

([A-Za-z])([A-Za-z0-9])* {if(Opt_T ==1){tokenString(id,yytext);}}


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
	
}

%%
int main( int argc, char **argv )
{
  if( argc != 2 ) {
    fprintf( stderr, "Usage: ./scanner [filename]\n" );
    exit(0);
  }
	
  FILE *fp = fopen( argv[1], "r" );
  if( fp == NULL ) {
    fprintf( stderr, "Open file error\n" );
    exit(-1);
  }
	
  yyin = fp;

  yylex();

  exit(0);
}
