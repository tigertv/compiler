#include <iostream>
#include "CodeGenerator.h"

CodeGenerator::CodeGenerator() {

}

void CodeGenerator::compile(Node* ast) {
	using namespace std;
	if (!ast) return;

	switch(ast->type) {
		case NodeType::SEQ:
			compile(ast->left);
			compile(ast->right);
			cout << ";" << endl;
			break;
			
		case NodeType::SET:
			cout << "\tint "; 
			compile(ast->left);

			variables.push_back(ast->left->value);

			cout << " = ";
			compile(ast->right);
			break;

		case NodeType::ADD_N:
			compile(ast->left);
			cout << " + ";
			compile(ast->right);
			break;

		case NodeType::VAR:
		case NodeType::NUMBER_C:
			cout << ast->value;
			break;

		case NodeType::PROG:
			cout << "#include <iostream>" << endl << endl;
			cout << "using namespace std;" << endl << endl;
			cout << "int main() {" << endl;
			compile(ast->left);
			
			for(int i=0;i<variables.size();i++) {
				cout << "\tcout << \"" << variables[i] << "=\" << " << variables[i] << " << endl;" << endl;
			}

			cout << "}" << endl;

		/*
		default:
			break;
		//*/

	}
}

