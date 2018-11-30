#include <iostream>
#include "CodeGenerator.h"

CodeGenerator::CodeGenerator() {
	outfile.open("output.s");
}

CodeGenerator::~CodeGenerator() {
	outfile.close();
}


void CodeGenerator::compile(Node* ast) {
	using namespace std;
	if (!ast) return;

	switch(ast->type) {
		case NodeType::SEQ:
			compile(ast->left);
			compile(ast->right);
			//outfile << ";" << endl;
			outfile << endl;
			break;
			
		case NodeType::SET:
			outfile << "mov eax,";
			compile(ast->left);

			variables.push_back(ast->left->value);

			compile(ast->right);

			outfile << endl;
			//outfile << "call print" << endl;

			//*
			outfile << "push eax" << endl;
			outfile << "push message2" << endl;
			outfile << "call printf" << endl;
			outfile << "add esp,8" << endl;
			//*/

			//outfile << "xor eax,eax" << endl;
			//outfile << "call print" << endl;
			break;

		case NodeType::ADD_N:
			compile(ast->left);
			outfile << endl;
			outfile << "add eax,";
			compile(ast->right);
			break;

		case NodeType::SUB_N:
			compile(ast->left);
			outfile << endl;
			outfile << "sub eax,";
			compile(ast->right);
			break;

		case NodeType::VAR:
			//outfile << ast->value;
			break;

		case NodeType::NUMBER_C:
			outfile << ast->value;
			break;

		case NodeType::PROG:
			outfile << "global _start" << endl;
			outfile << "extern printf" << endl;
			outfile << endl;

			outfile << "section .data" << endl;
			outfile << "message db 20" << endl;
			outfile << "message2 db \"%d\",10,0" << endl;
			outfile << "msg	db 0xa,\"123456789012345678901234567890123456789\", 0xa" << endl;
			outfile << "len	equ $-msg " << endl;
			outfile << endl;

			outfile << "section .text" << endl;
			outfile << "_start:" << endl;
			outfile << endl;

			compile(ast->left);

			// exit
			outfile << "xor eax,eax" << endl;
			outfile << "inc eax" << endl;
			outfile << "xor ebx, ebx" << endl;
			outfile << "int 0x80" << endl;
			outfile << endl;

			// print function
			outfile << "print:" << endl;
			outfile << endl;
			outfile << "xor edx, edx" << endl;

			outfile << "mov ecx, msg" << endl;
			outfile << "add ecx, eax" << endl;
			outfile << "xor eax, eax" << endl;
			outfile << "mov al, 4" << endl;
			outfile << "xor ebx, ebx" << endl;
			outfile << "inc ebx" << endl;
			outfile << "xor edx, edx" << endl;
			outfile << "mov dl, 1" << endl;
			//outfile << "mov dl, len" << endl;
			outfile << "int 0x80" << endl;
			outfile << "ret" << endl;
			
			/*
			for(int i=0;i<variables.size();i++) {
				outfile << "\toutfile << \"" << variables[i] << "=\" << " << variables[i] << " << endl;" << endl;
			}
			outfile << "" << endl;
			//*/

		/*
		default:
			break;
		//*/

	}
}

