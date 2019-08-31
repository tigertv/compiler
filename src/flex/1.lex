%{
	#include <stdio.h>
	#include <sstream>
	#include "../include/token.h"

	typedef union{
		int ival;
		char *cval;
	} yylval_type;

	extern yylval_type yylval;
	Token* token = nullptr;

	char* commentStart;
%}

%option nounput
%option yylineno
%option noyywrap
%x COMMENT_MULTI

%%

\/\/.*?\n		; // one-line comment

<INITIAL>"/*" { 
	/* begin of multi-line comment */
	//commentStart = yytext; 
	BEGIN(COMMENT_MULTI); 
}

<COMMENT_MULTI>"*/" { 
	/* end of multi-line comment */
	//char* comment = strndup(commentStart, yytext + 2 - commentStart);
	//printf("'%s': was a multi-line comment\n", comment);
	//free(comment); 
	BEGIN(INITIAL); 
}

<COMMENT_MULTI>. { 
	/* suppress whatever is in the comment */
}

"=="	{	
			yylval.cval = yytext;	
			return T_EQU;
		}
"!<"	{	
			yylval.cval = yytext;	
			return T_NLESS;
		}
"!>"	{	
			yylval.cval = yytext;	
			return T_NGREATER;
		}
"!="	{	
			yylval.cval = yytext;	
			return T_NEQU;
		}
"++"	{	
			yylval.cval = yytext;	
			return T_INC;
		}
"--"	{	
			yylval.cval = yytext;	
			return T_DEC;
		}
">"		{	
			yylval.cval = yytext;	
			return T_GREATER;
		}
"<"		{	
			yylval.cval = yytext;	
			return T_LESS;
		}
","		{	
			yylval.cval = yytext;	
			return T_COMMA;
		}
"="		{	
			yylval.cval = yytext;	
			return T_ASSIGN;
		}
";"		{	
			yylval.cval = yytext;	
			return T_SEMICOLON;
		}
"("		{	
			yylval.cval = yytext;	
			return T_LPAR;
		}
")"		{	
			yylval.cval = yytext;	
			return T_RPAR;
		}
"{"		{	
			yylval.cval = yytext;	
			return T_LBRACE;
		}
"}"		{	
			yylval.cval = yytext;	
			return T_RBRACE;
		}
"+"		{	
			yylval.cval = yytext;	
			return T_ADD;
		}
"-"		{	
			yylval.cval = yytext;	
			return T_SUB;
		}
"/"		{	
			yylval.cval = yytext;	
			return T_DIV;
		}
"*"		{	
			yylval.cval = yytext;	
			return T_MUL;
		}
fn		{	
			yylval.cval = yytext;	
			return T_FUNC;
		}
ret		{	
			yylval.cval = yytext;	
			return T_RET;
		}
for		{	
			yylval.cval = yytext;	
			return T_FOR;
		}
if		{	
			yylval.cval = yytext;	
			return T_IF;
		}
else	{	
			yylval.cval = yytext;	
			return T_ELSE;
		}
print	{	
			yylval.cval = yytext;	
			return T_PRINT;
		}
loop	{	
			yylval.cval = yytext;	
			return T_LOOP;
		}
while	{	
			yylval.cval = yytext;	
			return T_WHILE;
		}
do		{	
			yylval.cval = yytext;	
			return T_DO;
		}
true	{	
			yylval.cval = yytext;	
			return T_BOOL;
		}
false	{	
			yylval.cval = yytext;	
			return T_BOOL;
		}

[a-zA-Z][a-zA-Z0-9]*	{
							yylval.cval = yytext;	
							return T_ID;
						}

[0-9]+	{	
			yylval.ival = atoi(yytext);
			return T_NUMBER;
		}

[ \t\n]		{ // space 
			}

<<EOF>> {
			return 0;
		}

%%

