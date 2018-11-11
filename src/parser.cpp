#include <iostream>
#include "parser.h"

Parser::Parser() {

}

Token* Parser::nextToken() {
	ti++;
	if(ti < t->size()) {
		return t->at(ti);	
	}

	return nullptr;
}

Token* Parser::getCurrentToken() {
	if (ti < t->size()) {
		return t->at(ti);
	}
	return nullptr;
}

void Parser::printError(std::string text) {
	std::cout << "Error: " << text << std::endl;
	exit(1);
}

Node* Parser::parse(std::vector<Token*> *tokens) {
	using std::cout;
	using std::endl;

	t = tokens;
	Node* temp = nullptr;	
	Node* node = nullptr;

	do {
		temp = node;

		node = new Node();
		node->type = NodeType::SEQ;
		node->value = "SEQ";

		node->left = temp;
		node->right = statement();
	} while(nextToken());

	temp = node;
	node = new Node();
	node->type = NodeType::PROG;
	node->value = "PROG";
	node->left = temp;

	cout << "parse: end of parsing" << endl;

	return node;
}

Node* Parser::statement() {
	using std::cout;
	using std::endl;

	Node* node = nullptr;
	Token *token = getCurrentToken();

	switch(token->type) {
		case TokenType::IF: 
			std::cout << "if token" << std::endl;
			break;

		case TokenType::WHILE:
			break;

		case TokenType::DO:
			break;

		default:
			node = expression();
			token = getCurrentToken();
			
			if (!token) {
				printError("A token expected in statement.");
			}
			
			if (token->type != TokenType::SEMICOLON) {
				cout << "in statement token-type="+std::to_string(token->type) << endl;
				printError("A semicolon expected in statement.");
			}
			break;
	}

	return node;
}

// expression :: id = expression | id(params) | 
Node* Parser::expression() {
	using std::cout;
	using std::endl;
	Node* temp = nullptr;
	Node* node = term();

	if (!node) {
		cout << "A node is null in expression" << endl;
		return node;
		//printError("A node is null in expression");
	} 

	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in expression");
	} 

	if (node->type == NodeType::VAR && token->type == TokenType::EQU) {
		temp = node;
		node = new Node();
		node->type = NodeType::SET;
		nextToken();
		node->value = "SET";
		node->left = temp;
		node->right = expression();
	}

	if (node->type == NodeType::NUMBER_C && token->type == TokenType::ADD) {
		temp = node;
		node = new Node();
		node->type = NodeType::ADD_N;
		nextToken();
		node->value = "ADD";
		node->left = temp;
		node->right = expression();
	}

	return node;
}

Node* Parser::term() {
	Node* node = nullptr;
	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in term");
	}

	switch(token->type) {

		case TokenType::ID:
			node = new Node();
			nextToken();
			node->type = NodeType::VAR;
			node->value = token->value;
			break;

		case TokenType::NUMBER:
			node = new Node();
			nextToken();
			node->type = NodeType::NUMBER_C;
			node->value = token->value;
			break;

		case TokenType::STRING:
			node = new Node();
			nextToken();
			node->type = NodeType::STRING_C;
			node->value = token->value;
			break;
		
		default:
			//std::cout << "in term: another type of token value=" << token->value << " type=" << token->type << std::endl;
			//printError("Unexpected token in term");
			break;
	}

	return node;
}

Node* Parser::sum() {
	Node* node = term();
	Node* temp;
	Token *token = getCurrentToken();

	while(token->type == TokenType::ADD || token->type == TokenType::SUB) {
		temp = node;
		node = new Node();
		if (token->type == TokenType::ADD) {
			node->type = NodeType::ADD_N;
			node->value = "ADD";
		} else {
			node->type = NodeType::SUB_N;
			node->value = "SUB";
		}
		node->left = temp;
		node->right = term();
	}

	return node;
}

Node* block() {
	Node* node = new Node();

	return node;
}
