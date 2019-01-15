#ifndef FLEXER_H
#define FLEXER_H

#include <vector>
#include <string>
#include "token.h"
#include "../flex/clexer.h"

class Flexer {
public:
	Flexer();
	std::vector<Token*>* getTokens(std::string content);
};

#endif
