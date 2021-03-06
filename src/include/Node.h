#ifndef NODE_H
#define NODE_H

#include <string>
#include "SymbolTable.h"

enum NodeType {
	N_PROG=1,N_STATEMENT, N_NUMBER_C, N_STRING_C, N_ID, N_FUNC_CALL, N_FUNC, N_RET, N_ASSIGN, N_SEQ, N_CONDITION, N_ADD, N_SUB,
	N_PRINT, N_MUL, N_DIV, N_BLOCK, N_IF, N_EQU, N_NEQU, N_LESS, N_NLESS, N_GREATER, N_NGREATER, N_LOOP
};

class Node{
public:
	Node *left = nullptr;
	Node *right = nullptr;
	SymbolTable *symbolTable = nullptr;
	NodeType type;
	std::string value;
	Node *args = nullptr;
};

#endif
