#include <iostream>
#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(std::string ofile) {
	this->outfile.open(ofile);
}

CodeGenerator::~CodeGenerator() {
	outfile.close();
}


void CodeGenerator::compile(Node* ast) {
	using namespace std;
	if (!ast) return;

	switch(ast->type) {
		case NodeType::N_SEQ:
			compile(ast->left);
			compile(ast->right);
			outfile << endl;
			break;
			
		case NodeType::N_ASSIGN:
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

				if ( ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID)
					outfile << "mov eax, ";
				compile(ast->right);

				outfile << endl;
				if (!inVars) {
					outfile << "mov DWORD [ebp-" << 4*(size+1)  << "], eax" << endl;
				} else {
					outfile << "mov DWORD [ebp-" << 4*(i+1)  << "], eax" << endl;
				}
				outfile << endl;
			}

			break;

		case NodeType::N_ADD:
			if ( (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) 
			&& (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) ){
				outfile << "mov eax, ";
				compile(ast->left);

				outfile << endl;

				if (ast->right->value == "1") {
					outfile << "inc eax";
				} else {
					outfile << "add eax, ";
					compile(ast->right);
				}
				outfile << endl;

			} else if (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) {
				compile(ast->right);

				if (ast->left->value == "1") {
					outfile << "inc eax";
				} else {
					outfile << "add eax, ";
					compile(ast->left);
				}

				outfile << endl;

			} else if (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) {
				compile(ast->left);

				if (ast->right->value == "1") {
					outfile << "inc eax";
				} else {
					outfile << "add eax, ";
					compile(ast->right);
				}

				outfile << endl;

			} else {
				compile(ast->left);
				outfile << "push eax" << endl;
				compile(ast->right);
				outfile << "pop ebx" << endl;
				outfile  << "add eax, ebx" << endl;
			}
			break;

		case NodeType::N_SUB:
			if ( (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) 
			&& (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;

				if (ast->right->value == "1") {
					outfile << "dec eax";
				} else {
					outfile << "sub eax, ";
					compile(ast->right);
				}

				outfile << endl;

			} else if (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) {
				compile(ast->right);

				if (ast->left->value == "1") {
					outfile << "dec eax";
				} else {
					outfile << "sub eax, ";
					compile(ast->left);
				}

				outfile << endl;
				outfile << "neg eax" << endl;

			} else if (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) {
				compile(ast->left);

				if (ast->right->value == "1") {
					outfile << "dec eax";
				} else {
					outfile << "sub eax, ";
					compile(ast->right);
				}

				outfile << endl;

			} else {
				compile(ast->right);
				outfile << "push eax" << endl;
				compile(ast->left);
				outfile << "pop ebx" << endl;
				outfile  << "sub eax, ebx" << endl;
			}
			break;

		case NodeType::N_MUL:
			if ( (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) 
			&& (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl << "imul eax, ";
				compile(ast->right);
				outfile << endl;

			} else if (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) {
				compile(ast->right);
				outfile << "imul eax, ";
				compile(ast->left);
				outfile << endl;

			} else if (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) {
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

		case NodeType::N_DIV:
			if ( (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) 
			&& (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) ){
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "mov ebx, ";
				compile(ast->right);
				outfile << endl;
				outfile << "xor edx, edx" << endl;
				outfile << "idiv ebx" << endl;

			} else if (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) {
				compile(ast->right);
				outfile << "mov ebx, eax" << endl;
				outfile << "mov eax, ";
				compile(ast->left);
				outfile << endl;
				outfile << "xor edx, edx" << endl;
				outfile << "idiv ebx" << endl;

			} else if (ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID) {
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

		case NodeType::N_PRINT:
			if (ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID) {
				outfile << "mov eax, ";
			}

			compile(ast->left);

			outfile << endl;

			//*
			//outfile << "push eax" << endl;
			outfile << "call print" << endl;
			//*/

			/*
			outfile << "push eax" << endl;
			outfile << "push message2" << endl;
			outfile << "call printf" << endl;
			outfile << "add esp, 8" << endl;
			//*/
			break;

		case NodeType::N_ID:
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

		case NodeType::N_NUMBER_C:
			outfile << ast->value;
			break;

		case NodeType::N_FUNC:
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

		case NodeType::N_RET:
			if ( ast->left->type == NodeType::N_NUMBER_C || ast->left->type == NodeType::N_ID)
				outfile << "mov eax, ";
			compile(ast->left);
			break;

		case NodeType::N_FUNC_CALL:
			// prologue
			outfile << "call fn_" << ast->value << endl;
			break;

		case NodeType::N_PROG:
			// prologue
			outfile << "BITS 32" << endl;
			outfile << "global _start" << endl;
			//outfile << "extern printf" << endl;
			outfile << endl;

			//outfile << "section .data" << endl;
			//outfile << "message2 db \"%d\",10,0" << endl;
			//outfile << "msg TIMES 20 db 0" << endl;
			//outfile << "msg2 db 0" << endl;
			//outfile << "len	equ $-msg " << endl;
			//outfile << "section .bss" << endl;
			//outfile << "msg resb 40" << endl;
			//outfile << "msg2 resb 0" << endl;
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

			// print function : prints a decimal number in eax register with a new line
			outfile << "print:" << endl;
			outfile << "mov edi, 1" << endl;
			outfile << "mov ecx, esp" << endl;
			outfile << "mov ebx, 10" << endl;
			outfile << "dec ecx" << endl;
			outfile << "mov [ecx], bl" << endl;

			outfile << "print_loop:" << endl;
			outfile << "xor edx, edx" << endl;
			outfile << "idiv ebx" << endl;
			outfile << "add dl, '0'" << endl;
			outfile << "dec ecx" << endl;
			outfile << "inc edi" << endl;
			outfile << "mov [ecx],dl" << endl;
			outfile << "test eax, eax" << endl;
			outfile << "jnz print_loop" << endl;

			outfile << "mov eax, 4" << endl;
			outfile << "mov ebx, 1" << endl;
			outfile << "mov edx, edi" << endl;
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

