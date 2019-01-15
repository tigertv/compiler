#include <iostream>
#include "lexer.h"

Lexer::Lexer() {
	this->inToken = false;
}

void Lexer::checkID() {
	if (token->type == TokenType::T_ID) {
		if (token->value == "if") {
			token->type = TokenType::T_IF;
		} else if (token->value == "else") {
			token->type = TokenType::T_ELSE;
		} else if (token->value == "while") {
			token->type = TokenType::T_WHILE;
		} else if (token->value == "fn") {
			token->type = TokenType::T_FUNC;
		} else if (token->value == "ret") {
			token->type = TokenType::T_RET;
		} else if (token->value == "true") {
			token->type = TokenType::T_BOOL;
		} else if (token->value == "false") {
			token->type = TokenType::T_BOOL;
		} else if (token->value == "for") {
			token->type = TokenType::T_FOR;
		} else if (token->value == "do") {
			token->type = TokenType::T_DO;
		} else if (token->value == "print") {
			token->type = TokenType::T_PRINT;
		}
	}
}

std::vector<Token*>* Lexer::getTokens(std::string content) {
 
	std::vector<Token*> *result = new std::vector<Token*>();
	this->position = 0;

	//std::string breaksymbols[] = {"ddd","dss"};
	//std::string reservedWords[] = {"if","else", "while", "print"};
	//std::vector<std::string> symbolTable;

	std::string temp = "";
	int size = content.length();
	for (int i = 0; i<size;i ++) {
		char c = content[i];
		if (inToken) {
			if (token->type == TokenType::T_COMMENT) {
				if (c == '\n') {
					inToken = false;
				}
				continue;
			// create previous token
			} else if (token->type == TokenType::T_MULTICOMMENT) {
				if (c == '*') {
					if ( i+1 < size && content[i+1] == '/') {
						i = i+1;
						inToken = false;
					}
				}
				continue;
			
			} else if (c == '\"') {
				if (token->type == TokenType::T_STRING) {
					inToken = false;
					result->push_back(token);
					continue;
				} 

				this->checkID();	

			} else if (c == '+') {

				if (token->type == TokenType::T_ADD) {
					token->type = TokenType::T_INC;
					token->value += c;
					result->push_back(token);
					inToken = false;
					continue;
				} 

			} else if (c == '-') {

				if (token->type == TokenType::T_SUB) {
					token->type = TokenType::T_DEC;
					token->value += c;
					result->push_back(token);
					inToken = false;
					continue;
				} 

			} else if (c >= 'a' && c <= 'z') {

				if (token->type == TokenType::T_ID || token->type == TokenType::T_STRING) {
					token->value += c;
					continue;
				} 

			} else if (c >= '0' && c <= '9') {

				if (token->type == TokenType::T_NUMBER || token->type == TokenType::T_ID || token->type == TokenType::T_STRING) {
					token->value += c;
					continue;
				}

			} else {
				//other symbols
				if (token->type == TokenType::T_STRING) {
					token->value += c;
					continue;
				} 

				this->checkID();	

			}

			result->push_back(token);
		}

		if (c == '=') {
			token = new Token();
			token->type = TokenType::T_ASSIGN;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '(') {
			token = new Token();
			token->type = TokenType::T_LPAR;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == ')') {
			token = new Token();
			token->type = TokenType::T_RPAR;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '{') {
			token = new Token();
			token->type = TokenType::T_LBRACE;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '}') {
			token = new Token();
			token->type = TokenType::T_RBRACE;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '+') {
			token = new Token();
			token->type = TokenType::T_ADD;
			token->value = c;
			inToken = true;

		} else if (c == '-') {
			token = new Token();
			token->type = TokenType::T_SUB;
			token->value = c;
			inToken = true;

		} else if (c == '*') {
			token = new Token();
			token->type = TokenType::T_MUL;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '/') {
			if (i+1 < size) {
				if (content[i+1] == '/') {
					token = new Token();
					token->type = TokenType::T_COMMENT;
					token->value = "COMMENT";
					inToken = true;
					continue;
				} else if (content[i+1] == '*') {
					token = new Token();
					token->type = TokenType::T_MULTICOMMENT;
					token->value = "MULTICOMMENT";
					inToken = true;
					continue;
				}
			}

			token = new Token();
			token->type = TokenType::T_DIV;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == ';') {
			token = new Token();
			token->type = TokenType::T_SEMICOLON;
			token->value = c;
			result->push_back(token);
			inToken = false;

		} else if (c == '\"') {
			token = new Token();
			token->type = TokenType::T_STRING;
			token->value = "";
			inToken = true;

		} else if (c >= 'a' && c <= 'z') {
			token = new Token();
			token->type = TokenType::T_ID;
			token->value = c;
			inToken = true;

		} else if (c >= '0' && c <= '9') {
			token = new Token();
			token->type = TokenType::T_NUMBER;
			token->value = c;
			inToken = true;

		} else {
			// other symbols
			inToken = false;
		}

	}

	return result;
}
