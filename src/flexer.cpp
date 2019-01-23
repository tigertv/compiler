#include <iostream>
#include "flexer.h"

// TODO: move it
typedef union {
	int ival;
	char *cval;
} yylval_type;

yylval_type yylval;

Flexer::Flexer() {

}

std::vector<Token*>* Flexer::getTokens(std::string content) {
 	std::vector<Token*> *result = new std::vector<Token*>();
	int p = 0;
	Token *token; 

	YY_BUFFER_STATE buffer = yy_scan_string(content.c_str());
	while(p = yylex()) {
		token = new Token();
		token->type = (TokenType)p;
		switch(p) {
		case T_NUMBER:
			token->value = std::to_string(yylval.ival);
			result->push_back(token);
			break;

		case T_ID:
			token->value = yylval.cval;
			result->push_back(token);
			break;

		case T_FUNC:
			token->value = "FUNC";
			result->push_back(token);
			break;

		case T_RET:
			token->value = "RET";
			result->push_back(token);
			break;

		case T_DIV:
			token->value = "/";
			result->push_back(token);
			break;

		case T_MUL:
			token->value = "*";
			result->push_back(token);
			break;

		case T_SUB:
			token->value = "-";
			result->push_back(token);
			break;

		case T_ADD:
			token->value = "+";
			result->push_back(token);
			break;

		case T_LPAR:
			token->value = "(";
			result->push_back(token);
			break;

		case T_RPAR:
			token->value = ")";
			result->push_back(token);
			break;

		case T_LBRACE:
			token->value = "{";
			result->push_back(token);
			break;

		case T_RBRACE:
			token->value = "}";
			result->push_back(token);
			break;

		case T_ASSIGN:
			token->value = "=";
			result->push_back(token);
			break;

		case T_PRINT:
			token->value = "PRINT";
			result->push_back(token);
			break;

		case T_SEMICOLON:
			token->value = ";";//std::to_string(yylval.ival);
			result->push_back(token);
			break;

		case T_COMMA:
			token->value = ",";
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
