#ifndef CODE_GENERATOR_H 
#define CODE_GENERATOR_H 

#include "node.h"
#include <vector>
#include <string>
#include <fstream>

class CodeGenerator{
	std::vector<std::string> variables;
	std::ofstream outfile;
public:
	CodeGenerator();
	~CodeGenerator();
	void compile(Node* tree);
};

#endif
