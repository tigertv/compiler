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
			{
				outfile << "sub esp, 4" << endl;
				variables.push_back(ast->left->value);

				int size = variables.size();
				outfile << "mov eax, ";

				compile(ast->right);

				outfile << endl;
				outfile << "mov DWORD [ebp-" << 4*size  << "], eax" << endl;
				outfile << endl;
				//outfile << "call print" << endl;
			}

			//*
			outfile << "push eax" << endl;
			outfile << "push message2" << endl;
			outfile << "call printf" << endl;
			outfile << "add esp, 8" << endl;
			//*/

			//outfile << "xor eax,eax" << endl;
			//outfile << "call print" << endl;
			break;

		case NodeType::ADD_N:
			compile(ast->left);
			outfile << endl;
			outfile << "push eax" << endl;
			outfile << "mov eax, ";
			compile(ast->right);
			outfile << endl << "pop ebx";
			outfile << endl << "add eax, ebx";
			break;

		case NodeType::SUB_N:
			compile(ast->left);
			outfile << endl;
			outfile << "push eax" << endl;
			outfile << "mov eax, ";
			compile(ast->right);
			outfile << endl << "pop ebx";
			outfile << endl << "sub ebx, eax";
			outfile << endl << "mov eax, ebx";
			break;

		case NodeType::MUL_N:
			compile(ast->left);
			outfile << endl;
			outfile << "push eax" << endl;
			outfile << "mov eax, ";
			compile(ast->right);
			outfile << endl << "pop ebx";
			outfile << endl << "imul eax, ebx";
			break;

		case NodeType::DIV_N:
			compile(ast->left);
			outfile << endl;
			outfile << "xor edx, edx" << endl;
			outfile << "push eax" << endl;
			outfile << "mov eax, ";
			compile(ast->right);
			outfile << endl << endl;
			outfile << "mov ebx, eax" << endl;
			outfile << "pop eax" << endl;
			outfile << "idiv ebx";
			break;

		case NodeType::PRINT_N:
			if (ast->left) {
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "push eax" << endl;
				outfile << "push message2" << endl;
				outfile << "call printf" << endl;
				outfile << "add esp, 8" << endl;
			}
			break;

		case NodeType::ID_N:
			{
				bool isDefined = false;
				int size = variables.size();
				for(int i=0;i<size;i++) {
					if (variables[i] == ast->value) {
						outfile << "DWORD [ebp-" << 4*i+4 << "]";
						isDefined = true;
						break;
					}
				}

				if(!isDefined) {
					cout << "Unexpected variable \"" << ast->value << "\"" << endl;
					exit(1);
				}
			}
			break;

		case NodeType::NUMBER_C:
			outfile << ast->value;
			break;

		case NodeType::PROG:
			// prologue
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
			outfile << "_start:" << endl << endl;
			outfile << "push ebp" << endl;
			outfile << "mov ebp,esp" << endl;
			outfile << endl;

			compile(ast->left);

			// epilogue
			outfile << "pop ebp" << endl;
			outfile << endl;

			// exit
			outfile << "mov eax, 1" << endl;
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
			outfile << endl;

			
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

