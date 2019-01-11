#ifndef FLEXER_H
#define FLEXER_H

#include <vector>
#include <string>
#include "token.h"
#include "../flex/clexer.h"

class Flexer {
	bool inToken;
	Token *token;
	void checkID();
	int position;
public:
	Flexer();
	std::vector<Token*>* getTokens(std::string content);
};

#endif
