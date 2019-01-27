#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "token.h"
#include "node.h"
#include "SymbolTable.h"

class Parser {
	std::vector<Token*> *t;
	std::vector<SymbolTable*> scopes;
	Node* prog = nullptr;
	uint ti = 0;

	Token* nextToken();
	Token* getCurrentToken();
	void printError(std::string text);
	bool expect(TokenType type);

	Node* statements();
	Node* statement();
	Node* expression();
	Node* term();
	Node* factor();
	Node* block();
	Node* condition();
	Node* functionArgs();

public:
	Parser();
	Node* parse(std::vector<Token*>*tokens);
};

#endif
