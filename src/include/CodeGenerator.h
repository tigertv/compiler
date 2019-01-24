#ifndef CODE_GENERATOR_H 
#define CODE_GENERATOR_H 

#include <vector>
#include <string>
#include <fstream>
#include "node.h"
#include "SymbolTable.h"

class CodeGenerator{
	std::vector<std::string> variables;
	std::ofstream outfile;
	SymbolTable* currentSymbolTable = nullptr;
	Node* currentFunction = nullptr;
	int label = 0;

public:
	CodeGenerator(std::string ofile);
	~CodeGenerator();
	void compile(Node* tree);
};

#endif
