#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "token.h"
#include "node.h"

class Parser {
	std::vector<Token*> *t;
	std::vector<std::string> names;
	Node* prog = nullptr;
	uint ti = 0;

	Token* nextToken();
	Token* getCurrentToken();

	Node* statement();
	Node* expression();
	Node* term();
	Node* factor();
	Node* block();
	void printError(std::string text);
	bool expect(TokenType type);
public:
	Parser();
	Node* parse(std::vector<Token*>*tokens);

};

#endif
