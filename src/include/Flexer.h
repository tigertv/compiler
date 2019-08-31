#ifndef FLEXER_H
#define FLEXER_H

#include <vector>
#include <string>
#include "clexer.h"
#include "ILexer.h"
#include "Token.h"

class Flexer : public ILexer {
public:
	Flexer();
	virtual ~Flexer() {};
	std::vector<Token*>* getTokens(std::string content);
};

#endif
