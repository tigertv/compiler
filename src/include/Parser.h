#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

#include "Node.h"
#include "SymbolTable.h"
#include "Token.h"

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
	Node* conditionBlock();
	Node* functionArgs();

public:
	Parser();
	Node* parse(std::vector<Token*>*tokens);
};

#endif
