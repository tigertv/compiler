#include <iostream>
#include <fstream>
#include <string>
#include <list>
//#include "lexer.h"
#include "flexer.h"
#include "parser.h"
#include "ILexer.h"
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
	std::cout << "type: " << node->type << ", value: " << node->value << std::endl;
	if (node->left) traverse(node->left);
	if (node->right) traverse(node->right);
}

int main(int argc, char* argv[]) {
	using namespace std;

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " infile.calc outfile.asm" << endl;
        return 1;
    }
    // Print the user's name:

	string content = readFile(argv[1]);
	if (content.empty()) {
		cout << "Error: bad filename or no content" << endl;
		return 1;
	}

	cout << content;
	cout << "===============================" << endl;

	//Lexer lexer;
	Flexer lexer;
	vector<Token*> *tokens = lexer.getTokens(content);
	
	for (unsigned long int i=0; i < tokens->size(); i++) {
		cout << tokens->at(i)->type << " " << tokens->at(i)->value << endl;
	}

	Parser parser;
	Node* ast = parser.parse(tokens);
	cout << "+++++++++++++++++++++++++++++" << endl;

	traverse(ast);

	//*
	cout << "+++++ after traverse ast ++++" << endl;
	
	if (argv[2]) {
		CodeGenerator generator(argv[2]);
		generator.compile(ast);
	} else {
		CodeGenerator generator("output.asm");
		generator.compile(ast);
	}
	//*/

	//delete(tokens);
	//delete(lexer);
	return 0;
}
