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

"=="	return T_EQU;
"!<"	return T_NLESS;
"!>"	return T_NGREATER;
"!="	return T_NEQU;
"++"	return T_INC;
"--"	return T_DEC;
">"		return T_GREATER;
"<"		return T_LESS;
","		return T_COMMA;
"="		return T_ASSIGN;
";"		return T_SEMICOLON;
"("		return T_LPAR;
")"		return T_RPAR;
"{"		return T_LBRACE;
"}"		return T_RBRACE;
"+"		return T_ADD;
"-"		return T_SUB;
"/"		return T_DIV;
"*"		return T_MUL;
fn		return T_FUNC;
ret		return T_RET;
for		return T_FOR;
if		return T_IF;
print	return T_PRINT;
while	return T_WHILE;
do		return T_DO;
else	return T_ELSE;
true	return T_BOOL;
false	return T_BOOL;

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

