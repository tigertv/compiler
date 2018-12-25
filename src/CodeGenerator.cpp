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
				bool inVars = false;
				int size = variables.size();
				int i=0;
				for (;i<size;i++) {
					if (ast->left->value == variables[i]) {
						inVars = true;
						break;
					}
				}

				if (!inVars) {
					outfile << "sub esp, 4" << endl;
					variables.push_back(ast->left->value);
				}

				if ( ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N)
					outfile << "mov eax, ";
				compile(ast->right);

				outfile << endl;
				if (!inVars) {
					outfile << "mov DWORD [ebp-" << 4*(size+1)  << "], eax" << endl;
				} else {
					outfile << "mov DWORD [ebp-" << 4*(i+1)  << "], eax" << endl;
				}
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
			if ( (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) 
			&& (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl << "add eax, ";
				compile(ast->right);
				outfile << endl;

			} else if (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) {
				compile(ast->right);
				outfile << "add eax, ";
				compile(ast->left);
				outfile << endl;

			} else if (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) {
				compile(ast->left);
				outfile << "add eax, ";
				compile(ast->right);
				outfile << endl;

			} else {
				compile(ast->left);
				outfile << "push eax" << endl;
				compile(ast->right);
				outfile << "pop ebx" << endl;
				outfile  << "add eax, ebx" << endl;
			}
			break;

		case NodeType::SUB_N:
			if ( (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) 
			&& (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "sub eax, ";
				compile(ast->right);
				outfile << endl;

			} else if (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) {
				compile(ast->right);
				outfile << "sub eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "neg eax" << endl;

			} else if (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) {
				compile(ast->left);
				outfile << "sub eax, ";
				compile(ast->right);
				outfile << endl;

			} else {
				compile(ast->right);
				outfile << "push eax" << endl;
				compile(ast->left);
				outfile << "pop ebx" << endl;
				outfile  << "sub eax, ebx" << endl;
			}
			break;

		case NodeType::MUL_N:
			if ( (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) 
			&& (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl << "imul eax, ";
				compile(ast->right);
				outfile << endl;

			} else if (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) {
				compile(ast->right);
				outfile << "imul eax, ";
				compile(ast->left);
				outfile << endl;

			} else if (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) {
				compile(ast->left);
				outfile << "imul eax, ";
				compile(ast->right);
				outfile << endl;

			} else {
				compile(ast->left);
				outfile << "push eax" << endl;
				compile(ast->right);
				outfile << "pop ebx" << endl;
				outfile  << "imul eax, ebx" << endl;
			}
			break;

		case NodeType::DIV_N:
			if ( (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) 
			&& (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "mov ebx, ";
				compile(ast->right);
				outfile << endl;
				outfile << "xor edx, edx" << endl;
				outfile << "idiv ebx" << endl;

			} else if (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) {
				compile(ast->right);
				outfile << "mov ebx, eax" << endl;
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "xor edx, edx" << endl;
				outfile << "idiv ebx" << endl;

			} else if (ast->right->type == NodeType::NUMBER_C || ast->right->type == NodeType::ID_N) {
				compile(ast->left);
				outfile << "mov ebx, ";
				compile(ast->right);
				outfile << endl;
				outfile << "xor edx, edx" << endl;
				outfile << "idiv ebx" << endl;

			} else {
				compile(ast->right);
				outfile << "push eax" << endl;
				compile(ast->left);
				outfile << "pop ebx" << endl;
				outfile << "xor edx, edx" << endl;
				outfile  << "idiv ebx" << endl;
			}
			break;

		case NodeType::PRINT_N:
			if (ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N) {
				outfile << "mov eax, ";
			}

			compile(ast->left);

			outfile << endl;
			outfile << "push eax" << endl;
			outfile << "push message2" << endl;
			outfile << "call printf" << endl;
			outfile << "add esp, 8" << endl;
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

		case NodeType::FUNC_N:
			compile(ast->right);

			// prologue
			outfile << "fn_" << ast->value << ":" << endl << endl;
			outfile << "push ebp" << endl;
			outfile << "mov ebp, esp" << endl << endl;

			compile(ast->left);

			// epilogue
			outfile << "mov esp, ebp" << endl;
			outfile << "pop ebp" << endl;
			outfile << "ret" << endl;
			outfile << endl;
			break;

		case NodeType::RET_N:
			if ( ast->left->type == NodeType::NUMBER_C || ast->left->type == NodeType::ID_N)
				outfile << "mov eax, ";
			compile(ast->left);
			break;

		case NodeType::FUNC_CALL:
			// prologue
			outfile << "call fn_" << ast->value << endl;
			break;

		case NodeType::PROG:
			// prologue
			outfile << "BITS 32" << endl;
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

			outfile << ";==== FUNCTIONS ======" << endl;
			compile(ast->right);
			outfile << ";==== FUNCTIONS ======" << endl;


			outfile << "_start:" << endl << endl;
			outfile << "push ebp" << endl;
			outfile << "mov ebp,esp" << endl;
			outfile << endl;

			compile(ast->left);

			// epilogue
			outfile << "mov esp, ebp" << endl;
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

