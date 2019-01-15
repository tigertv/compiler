#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "ILexer.h"
#include "token.h"

class Lexer : public ILexer {
	bool inToken;
	Token *token;
	void checkID();
	int position;
public:
	Lexer();
	std::vector<Token*>* getTokens(std::string content);
};

#endif
