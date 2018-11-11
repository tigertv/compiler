#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "token.h"

class Lexer {
	bool inToken;
	Token *token;
	void checkID();
public:
	Lexer();
	std::vector<Token*>* getTokens(std::string content);
};

#endif
