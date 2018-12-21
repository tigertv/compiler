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

bool Parser::expect(TokenType type) {
	
	Token* token = getCurrentToken();
	if (token->type == type) {
		nextToken();
		return true;
	}

	return false;
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
			
		case TokenType::PRINT: 
			node = new Node();
			node->type = NodeType::PRINT_N;
			node->value = "PRINT";

			token = nextToken();
			if (token->type != TokenType::LPAR) {
				printError("A left parenthesis expected in print statement.");
			}

			nextToken();

			node->left = expression();
			token = getCurrentToken();
			
			if (token->type != TokenType::RPAR) {
				printError("A right parenthesis expected in print statement.");
			}

			token = nextToken();
			if (token->type != TokenType::SEMICOLON) {
				printError("A semicolon expected in print statement.");
			}
			break;

		case TokenType::WHILE:
			break;

		case TokenType::DO:
			break;

		case TokenType::ID:
			{
				Node* temp = new Node();
				temp->type = NodeType::ID_N;
				temp->value = token->value;
				nextToken();

				// assign statement
				if (this->expect(TokenType::EQU) ) {
					node = new Node();
					node->type = NodeType::SET;
					node->value = "SET";
					node->left = temp;
					node->right = expression();

				// function call statement
				} else if (this->expect(TokenType::LPAR) ) {
					node = new Node();
					node->type = NodeType::FUNC_CALL;
					node->value = "FUNC_CALL";
					node->left = temp;
					node->right = expression();

					if(this->expect(TokenType::RPAR) ) {

						if (!this->expect(TokenType::SEMICOLON)) {
							printError("A semicolon expected in statement.");
						}

					} else {
						printError("A right parenthesis expected in statement.");
					}

				} else {
					printError("A left parenthesis or equal are expected in statement.");
				}
			}
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

Node* Parser::expression() {
	using std::cout;
	using std::endl;

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

	Node* temp;

	while (token->type == TokenType::ADD || token->type == TokenType::SUB) {
		//cout << "in expr loop" << endl;
		temp = node;
		node = new Node();

		if (token->type == TokenType::ADD) {
			node->type = NodeType::ADD_N;
			node->value = "ADD";
		} else {
			node->type = NodeType::SUB_N;
			node->value = "SUB";
		}
		node->left=temp;
		nextToken();
		node->right = term();
		token = getCurrentToken();
	}

	return node;
}

Node* Parser::factor() {
	Node* node = nullptr;
	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in factor");
	}

	switch(token->type) {

		case TokenType::ID:
			node = new Node();
			nextToken();
			node->type = NodeType::ID_N;
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

		case TokenType::LPAR:
			nextToken();
			node = expression();

			if(!this->expect(TokenType::RPAR) ) {
				printError("A right parenthesis expected in statement.");
			}

			break;
			
		default:
			//std::cout << "in term: another type of token value=" << token->value << " type=" << token->type << std::endl;
			//printError("Unexpected token in term");
			break;
	}

	return node;
}

Node* Parser::term() {
	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in term");
	}

	Node* node = factor();
	Node* temp;
	//std::cout << "in term's 1=" << token->type << std::endl;
	token = getCurrentToken();
	//std::cout << "in term's 3=" << token->type << std::endl;

	while (token->type == TokenType::DIV || token->type == TokenType::MUL) {
		temp = node;
		node = new Node();

	//	std::cout << "in term's 2" << std::endl;

		if (token->type == TokenType::MUL) {
			node->type = NodeType::MUL_N;
			node->value = "MUL";
		} else {
			node->type = NodeType::DIV_N;
			node->value = "DIV";
		}
		node->left=temp;
		nextToken();
		node->right = factor();
		token = getCurrentToken();
	}

	return node;
}
/*
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

//*/
