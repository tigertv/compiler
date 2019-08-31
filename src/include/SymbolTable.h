#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>

#include "Token.h"

class SymbolTable {
	std::vector<std::string> symbols;
public:
	bool isSymbolExist(std::string symbol);
	bool addSymbol(std::string symbol);
	int getSize();
	int getSymbolIndex(std::string symbol);
};

#endif
