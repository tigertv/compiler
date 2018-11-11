#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "token.h"
#include "node.h"

class Parser {
	std::vector<Token*> *t;
	uint ti = 0;

	Token* nextToken();
	Token* getCurrentToken();

	Node* statement();
	Node* expression();
	Node* term();
	Node* sum();
	Node* block();
	void printError(std::string text);
public:
	Parser();
	Node* parse(std::vector<Token*>*tokens);

};

#endif
