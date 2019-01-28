#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
	T_ID=1, T_ASSIGN, T_ENDOFSTATEMENT, T_NUMBER, T_SEMICOLON, T_LPAR, T_RPAR, T_LBRACE, T_FOR, 
	T_RBRACE, T_ADD, T_SUB, T_MUL, T_DIV, T_STRING, T_IF, T_ELSE, T_DO, T_WHILE, T_FUNC, T_INC, T_DEC, T_RET, T_BOOL,
	T_PRINT, T_COMMENT, T_MULTICOMMENT, T_COMMA, T_EQU, T_NLESS, T_NGREATER, T_NEQU, T_GREATER, T_LESS, T_LOOP 
};

struct Token{
	TokenType type;
	std::string value;
};

#endif
