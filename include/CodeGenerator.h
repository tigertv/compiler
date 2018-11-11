#ifndef CODE_GENERATOR_H 
#define CODE_GENERATOR_H 

#include "node.h"
#include <vector>
#include <string>

class CodeGenerator{
	std::vector<std::string> variables;
public:
	CodeGenerator();
	void compile(Node* tree);
};

#endif
