#include "SymbolTable.h"

bool SymbolTable::isSymbolExist(std::string symbol) {
	int size = this->symbols.size();
	for (int i=0;i<size;i++) {
		if (symbol == this->symbols[i]) return true;
	}
	return false;
}

bool SymbolTable::addSymbol(std::string symbol) {
	if (this->isSymbolExist(symbol)) return false;
	this->symbols.push_back(symbol);	
	return true;
}

int SymbolTable::getSize() {
	return this->symbols.size();	
}

int SymbolTable::getSymbolIndex(std::string symbol) {
	int size = this->symbols.size();
	for (int i=0;i<size;i++) {
		if (symbol == this->symbols[i]) return i;
	}
	return -1;
}
