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

"++"	return INC;
"--"	return DEC;
"="		return EQU;
";"		return SEMICOLON;
"("		return LPAR;
")"		return RPAR;
"{"		return LBRACE;
"}"		return RBRACE;
"+"		return ADD;
"-"		return SUB;
"/"		return DIV;
"*"		return MUL;
fn		return FUNC;
ret		return RET;
for		return FOR;
print	return PRINT;
while	return WHILE;
do		return DO;
else	return ELSE;
true	return BOOL;
false	return BOOL;

[a-zA-Z][a-zA-Z0-9]*	{
							yylval.cval = yytext;	
							return ID;
						}

[0-9]+	{	
			yylval.ival = atoi(yytext);
			return NUMBER;
		}

[ \t\n]		; // space

<<EOF>> {
			return 0;
		}

%%

