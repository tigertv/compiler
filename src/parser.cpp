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

	Node* prog = new Node();
	prog->type = NodeType::N_PROG;
	prog->value = "PROG";
	prog->symbolTable = new SymbolTable();
	this->scopes.push_back(prog->symbolTable);

	this->prog = prog;
	prog->left = statements();

	this->scopes.pop_back();

	cout << "parse: end of parsing" << endl;

	return prog;
}

Node* Parser::statements() {

	/*
	Node* temp;
	Token* token;
	Node* node = nullptr;

	do {
		temp = node;

		Node* st = statement();
		if (st) {
			node = new Node();
			node->type = NodeType::N_SEQ;
			node->value = "SEQ";

			node->left = temp;
			node->right = st;
		}
		token = getCurrentToken();
	} while(token);
	//*/

	////////////////////////////////

	Node* temp;
	Node* node = nullptr;
	Token* token = getCurrentToken();

	while(token && token->type != TokenType::T_RBRACE) {
		temp = node;

		node = new Node();
		node->type = NodeType::N_SEQ;
		node->value = "SEQ";
		node->left = temp;
		node->right = statement();
		token = getCurrentToken();
	}

	return node;
}


Node* Parser::statement() {
	using std::cout;
	using std::endl;

	Node* node = nullptr;
	Token *token = getCurrentToken();
	if (!token) {
		printError("A token expected in statement");
	}

	switch(token->type) {

		case TokenType::T_FUNC: 
			{
				token = nextToken();

				if (!token || token->type != TokenType::T_ID) {
					printError("An id expected in function statement.");
				}

				node = new Node();
				node->type = NodeType::N_FUNC;
				node->value = token->value; // id

				nextToken();
				
				if (!expect(TokenType::T_LPAR)) {
					printError("A left parenthes expected in function statement.");
				}

				////////////////////////////////////////////

				Node* func = node;
				Node* temp = node;
				token = getCurrentToken();

				while(token && token->type == TokenType::T_ID) {
					node = new Node();
					node->type = NodeType::N_ID; // FUNC_ARG
					node->value = token->value;

					temp->args = node;
					temp = temp->args;

					nextToken();

					if (!expect(TokenType::T_COMMA)) {
						break;
					}

					token = getCurrentToken();
				}

				////////////////////////////////////////////

				if (!expect(TokenType::T_RPAR)) {
					printError("A right parenthes expected in function statement.");
				}

				func->left = block();
				temp = prog->right;
				prog->right = func;
				func->right = temp;
				node = nullptr;
			}

			break;

		case TokenType::T_RET: 
			{
				nextToken();

				node = new Node();
				node->type = NodeType::N_RET;
				node->value = "RET";
				node->left = expression();

				if (!node->left) {
					printError("An expression expected in return statement.");
				}

				if (!expect(TokenType::T_SEMICOLON)) {
					printError("A right brace expected in return statement.");
				}
			}
			break;
			
		case TokenType::T_IF: 
			{
				node = new Node();
				node->type = NodeType::N_IF;
				node->value = "IF";
				nextToken();

				if (!expect(TokenType::T_LPAR)) {
					printError("A left parenthes expected in if-statement.");
				} 

				node->left = this->condition();
				
				if (!expect(TokenType::T_RPAR)) {
					printError("A right parenthes expected in if-statement.");
				} 

				node->right = block();
			}
			break;
			
		case TokenType::T_PRINT: 
			node = new Node();
			node->type = NodeType::N_PRINT;
			node->value = "PRINT";
			nextToken();

			if (!expect(TokenType::T_LPAR)) {
				printError("A left parenthesis expected in print statement.");
			}

			node->left = expression();
			
			if (!expect(TokenType::T_RPAR)) {
				printError("A right parenthesis expected in print statement.");
			}

			if (!expect(TokenType::T_SEMICOLON)) {
				printError("A semicolon expected in print statement.");
			}

			break;

		case TokenType::T_WHILE:
			break;

		case TokenType::T_DO:
			break;

		case TokenType::T_ID:
			{
				Node* temp = new Node();
				temp->type = NodeType::N_ID;
				temp->value = token->value;
				nextToken();

				// assign statement
				if (this->expect(TokenType::T_ASSIGN) ) {
					node = new Node();
					node->type = NodeType::N_ASSIGN;
					node->value = "SET";
					node->left = temp;
					// add to symbol table if it doesn't exist
					SymbolTable* table = this->scopes.back();
					if (!table->isSymbolExist(temp->value) ) {
						table->addSymbol(temp->value); 
					}

					node->right = expression();

					if (!this->expect(TokenType::T_SEMICOLON)) {
						printError("A semicolon expected in statement.");
					}

				// function call statement
				} else if (this->expect(TokenType::T_LPAR) ) {
					node = new Node();
					node->type = NodeType::N_FUNC_CALL;
					node->value = token->value;

					node->args = this->functionArgs();

					if(this->expect(TokenType::T_RPAR) ) {

						if (!this->expect(TokenType::T_SEMICOLON)) {
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
			
			if (token->type != TokenType::T_SEMICOLON) {
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
		return node;
	} 

	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in expression");
	} 

	Node* temp;

	while (token->type == TokenType::T_ADD || token->type == TokenType::T_SUB) {
		temp = node;
		node = new Node();

		if (token->type == TokenType::T_ADD) {
			node->type = NodeType::N_ADD;
			node->value = "ADD";
		} else {
			node->type = NodeType::N_SUB;
			node->value = "SUB";
		}
		node->left=temp;
		nextToken();
		node->right = term();
		token = getCurrentToken();
	}

	return node;
}

Node* Parser::functionArgs() {

	Node* temp;
	Node* tail = expression();
	
	Token* token = getCurrentToken();

	while(token && token->type == TokenType::T_COMMA) {
		nextToken();
		temp = expression();
		temp->args = tail;
		tail = temp;
		token = getCurrentToken();
	}

	return tail;
}

Node* Parser::factor() {
	Node* node = nullptr;
	Token *token = getCurrentToken();

	if (!token) {
		printError("A token expected in factor");
	}

	switch(token->type) {

		case TokenType::T_ID:
			{
				node = new Node();
				Token* ttoken = nextToken();
				if (ttoken->type == TokenType::T_LPAR) {
					nextToken();
					node->type = NodeType::N_FUNC_CALL;
					node->value = token->value;

					node->args = this->functionArgs();
					
					if(!this->expect(TokenType::T_RPAR) ) {
						printError("A right parenthesis expected in funcion call.");
					}

				} else {
					node->type = NodeType::N_ID;
					node->value = token->value;
				}
			}
			break;

		case TokenType::T_NUMBER:
			node = new Node();
			nextToken();
			node->type = NodeType::N_NUMBER_C;
			node->value = token->value;
			break;

		case TokenType::T_STRING:
			node = new Node();
			nextToken();
			node->type = NodeType::N_STRING_C;
			node->value = token->value;
			break;

		case TokenType::T_LPAR:
			nextToken();
			node = expression();

			if(!this->expect(TokenType::T_RPAR) ) {
				printError("A right parenthesis expected in statement.");
			}

			break;
			
		case TokenType::T_RPAR:
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
	token = getCurrentToken();

	while (token->type == TokenType::T_DIV || token->type == TokenType::T_MUL) {
		temp = node;
		node = new Node();

		if (token->type == TokenType::T_MUL) {
			node->type = NodeType::N_MUL;
			node->value = "MUL";
		} else {
			node->type = NodeType::N_DIV;
			node->value = "DIV";
		}
		node->left=temp;
		nextToken();
		node->right = factor();
		token = getCurrentToken();
	}

	return node;
}

Node* Parser::block() {

	if (!expect(TokenType::T_LBRACE)) {
		printError("A left brace expected in block.");
	}

	Node* block = new Node();
	block->type = NodeType::N_BLOCK;
	block->value = "BLOCK"; 
	block->symbolTable = new SymbolTable();
	block->right = nullptr;

	this->scopes.push_back(block->symbolTable);

	Node* st = statements();

	if (!expect(TokenType::T_RBRACE)) {
		printError("A right brace expected in block.");
	}

	this->scopes.pop_back();

	block->left = st;
	return block;
}

Node* Parser::condition() {

	Node* node = new Node();
	node->left = expression();
	Token* token = getCurrentToken();

	switch(token->type) {
	case TokenType::T_EQU:
		node->type = NodeType::N_EQU;
		node->value = "EQU";
		break;

	case TokenType::T_NEQU:
		node->type = NodeType::N_NEQU;
		node->value = "NEQU";
		break;

	case TokenType::T_LESS:
		node->type = NodeType::N_LESS;
		node->value = "LESS";
		break;

	case TokenType::T_NLESS:
		node->type = NodeType::N_NLESS;
		node->value = "NLESS";
		break;

	case TokenType::T_GREATER:
		node->type = NodeType::N_GREATER;
		node->value = "GREATER";
		break;

	case TokenType::T_NGREATER:
		node->type = NodeType::N_NGREATER;
		node->value = "NGREATER";
		break;

	default:
		printError("A bool operators expected in condition.");
		break;
	}

	nextToken();
	node->right = expression();

	return node;
}
