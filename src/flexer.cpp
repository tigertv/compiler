#include <iostream>
#include "flexer.h"

// TODO: move it
typedef union {
	int ival;
	char *cval;
} yylval_type;

yylval_type yylval;

Flexer::Flexer() {
	this->inToken = false;
}

void Flexer::checkID() {
	if (token->type == TokenType::ID) {
		if (token->value == "if") {
			token->type = TokenType::IF;
		} else if (token->value == "else") {
			token->type = TokenType::ELSE;
		} else if (token->value == "while") {
			token->type = TokenType::WHILE;
		} else if (token->value == "fn") {
			token->type = TokenType::FUNC;
		} else if (token->value == "ret") {
			token->type = TokenType::RET;
		} else if (token->value == "true") {
			token->type = TokenType::BOOL;
		} else if (token->value == "false") {
			token->type = TokenType::BOOL;
		} else if (token->value == "for") {
			token->type = TokenType::FOR;
		} else if (token->value == "do") {
			token->type = TokenType::DO;
		} else if (token->value == "print") {
			token->type = TokenType::PRINT;
		}
	}
}

std::vector<Token*>* Flexer::getTokens(std::string content) {
 	std::vector<Token*> *result = new std::vector<Token*>();
	this->position = 0;
	int p = 0;
	Token *token; 

	YY_BUFFER_STATE buffer = yy_scan_string(content.c_str());
	while(p = yylex()) {
		token = new Token();
		token->type = (TokenType)p;
		switch(p) {
		case NUMBER:
			token->value = std::to_string(yylval.ival);
			result->push_back(token);
			break;

		case ID:
			token->value = yylval.cval;
			result->push_back(token);
			break;

		case FUNC:
			token->value = "FUNC";
			result->push_back(token);
			break;

		case RET:
			token->value = "RET";
			result->push_back(token);
			break;

		case DIV:
			token->value = "/";
			result->push_back(token);
			break;

		case MUL:
			token->value = "*";
			result->push_back(token);
			break;

		case SUB:
			token->value = "-";
			result->push_back(token);
			break;

		case ADD:
			token->value = "+";
			result->push_back(token);
			break;

		case LPAR:
			token->value = "(";
			result->push_back(token);
			break;

		case RPAR:
			token->value = ")";
			result->push_back(token);
			break;

		case LBRACE:
			token->value = "{";
			result->push_back(token);
			break;

		case RBRACE:
			token->value = "}";
			result->push_back(token);
			break;

		case EQU:
			token->value = "=";
			result->push_back(token);
			break;

		case PRINT:
			token->type = (TokenType)p;
			token->value = "PRINT";
			result->push_back(token);
			break;

		case SEMICOLON:
			token->type = TokenType::SEMICOLON;
			token->value = ";";//std::to_string(yylval.ival);
			result->push_back(token);
			break;

		default:
			break;
		}

		yylval.cval = "";
		yylval.ival = 0;
	}

	yy_delete_buffer(buffer);

	return result;
}
