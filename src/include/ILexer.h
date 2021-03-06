#ifndef ILEXER_H
#define ILEXER_H

#include <vector>
#include <string>

#include "Token.h"

struct ILexer
{
	virtual std::vector<Token*>* getTokens(std::string content) = 0;
	virtual ~ILexer() {};
};

#endif
