#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
	ID=1, EQU, ENDOFSTATEMENT, NUMBER, SEMICOLON, LPAR, RPAR, LBRACE, FOR, 
	RBRACE, ADD, SUB, MUL, DIV, STRING, IF, ELSE, DO, WHILE, FUNC, INC, DEC, RET, BOOL
};

struct Token{
	TokenType type;
	std::string value;
};

#endif
