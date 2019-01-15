#ifndef NODE_H
#define NODE_H

#include <string>

enum NodeType {
	N_PROG=1,N_STATEMENT, N_NUMBER_C, N_STRING_C, N_ID, N_FUNC_CALL, N_FUNC, N_RET, N_ASSIGN, N_SEQ, N_CONDITION, N_ADD, N_SUB,
	N_PRINT, N_MUL, N_DIV
};

class Node{
public:
	Node *left = nullptr;
	Node *right = nullptr;
	NodeType type;
	std::string value;
};

#endif
