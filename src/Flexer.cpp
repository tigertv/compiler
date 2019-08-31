#include "include/Flexer.h"

#include <iostream>

// TODO: remove it or change
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

	YY_BUFFER_STATE buff = yy_scan_string(content.c_str());
	while( (p = yylex()) != 0) {
		token = new Token();
		token->type = (TokenType)p;

		switch(p) {
		case T_NUMBER:
			token->value = std::to_string(yylval.ival);
			result->push_back(token);
			break;

		default:
			token->value = yylval.cval;
			result->push_back(token);
			break;
		}

		yylval.cval = (char*)"";
		yylval.ival = 0;
	}

	yy_delete_buffer(buff);

	return result;
}
