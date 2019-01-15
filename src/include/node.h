#ifndef NODE_H
#define NODE_H

#include <string>

enum NodeType {
	PROG=1,STATEMENT, NUMBER_C, STRING_C, ID_N, FUNC_CALL, FUNC_N, RET_N, SET, SEQ, CONDITION, ADD_N, SUB_N,
	PRINT_N, MUL_N, DIV_N
};

class Node{
public:
	Node *left = nullptr;
	Node *right = nullptr;
	NodeType type;
	std::string value;
};

#endif
