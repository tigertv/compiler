#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "lexer.h"
#include "parser.h"
#include "CodeGenerator.h"

std::string readFile(std::string fileName) {
	std::string line;
	std::string content;
	std::ifstream myfile(fileName);

	if (myfile.is_open()) {
		while(std::getline(myfile,line)) {
			content += line + '\n';
		}
		myfile.close();
	}

	return content;
}

void traverse(Node* node) {
	using namespace std;
	
	cout << "type: " << node->type << ", value: " << node->value << endl;
	if (node->left) traverse(node->left);
	if (node->right) traverse(node->right);
}

int main() {
	using namespace std;
	string content = readFile("source.calc");
	cout << content;
	cout << "===============================" << endl;

	Lexer lexer;
	vector<Token*> *tokens = lexer.getTokens(content);
	
	for (uint i=0;i<tokens->size();i++) {
		cout << tokens->at(i)->type << " " << tokens->at(i)->value << endl;
	}

	Parser parser;
	Node* ast = parser.parse(tokens);
	cout << "+++++++++++++++++++++++++++++" << endl;

	traverse(ast);

	cout << "+++++ after traverse ast ++++" << endl;

	CodeGenerator generator;
	generator.compile(ast);

	//delete(tokens);
	//delete(lexer);
	return 0;
}
