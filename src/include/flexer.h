#ifndef FLEXER_H
#define FLEXER_H

#include <vector>
#include <string>
#include "token.h"
//#include "../flex/clexer.h"
#include "clexer.h"
#include "ILexer.h"

class Flexer : public ILexer {
public:
	Flexer();
	virtual ~Flexer() {};
	std::vector<Token*>* getTokens(std::string content);
};

#endif
