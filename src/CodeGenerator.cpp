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
		case NodeType::N_LOOP:	{
			Node* condition = ast->args;

			outfile << "; LOOP COND" << endl;

			if ( condition->type == NodeType::N_NUMBER_C || condition->type == NodeType::N_ID)
				outfile << "mov eax, ";
			compile(condition);
			outfile << endl;
			outfile << "mov ecx, eax" << endl;

			int labelCount = ++this->label;

			// block
			outfile << ".L"<< labelCount << ": ; LOOP BEGIN" << endl;
			outfile << "push ecx" << endl;

			compile(ast->left);

			outfile << "pop ecx" << endl;
			outfile << "dec ecx" << endl;
			outfile << "jnz .L"<< labelCount << " ; LOOP END" << endl;
			outfile << endl;
		}
			break;

		case NodeType::N_SEQ:
			{
				compile(ast->left);
				compile(ast->right);
				outfile << endl;
			}
			break;

		case NodeType::N_IF:
			{
				outfile << "; IF " << endl;

				Node* condition = ast->args;

				if ( condition->left->type == NodeType::N_NUMBER_C || condition->left->type == NodeType::N_ID)
					outfile << "mov eax, ";
				compile(condition->left);
				outfile << endl;
				outfile << "mov edx, eax" << endl;

				if ( condition->right->type == NodeType::N_NUMBER_C || condition->right->type == NodeType::N_ID)
					outfile << "mov eax, ";
				compile(condition->right);
				outfile << endl;
				outfile << "cmp edx, eax" << endl;

				this->label++;

				switch(condition->type) {
				case NodeType::N_EQU:
					outfile << "jnz .L" << this->label << endl;
					break;
					
				case NodeType::N_NEQU:
					outfile << "jz .L" << this->label << endl;
					break;
					
				case NodeType::N_LESS:
					outfile << "jnl .L" << this->label << endl;
					break;

				case NodeType::N_NLESS:
					outfile << "jl .L" << this->label << endl;
					break;

				case NodeType::N_GREATER:
					outfile << "jng .L" << this->label << endl;
					break;

				case NodeType::N_NGREATER:
					outfile << "jg .L" << this->label << endl;
					break;

				default:
					cout << "Error in compile if-condition" << endl;
					exit(1);
					break;
				}

				int endLabel = this->label;
				compile(ast->left);

				if (ast->right) {
					outfile << "jmp .LE"<< this->label << endl;
				}

				outfile << ".L"<< this->label << ": ; END IF" << endl;
				// ELSE begins
				if (ast->right) {
					compile(ast->right);
					outfile << ".LE"<< endLabel << ": ; END ELSE-IF" << endl;
				}
			}
			break;

		case NodeType::N_BLOCK:
			{
				SymbolTable *temp = this->currentSymbolTable;
				this->currentSymbolTable = ast->symbolTable;

				outfile << "push ebp" << endl;
				outfile << "mov ebp, esp" << endl;
				outfile << "sub esp, " << 4 * this->currentSymbolTable->getSize() << endl;

				compile(ast->left);

				outfile << "leave" << endl;
				this->currentSymbolTable = temp;
			}
			break;
			
		case NodeType::N_ASSIGN:
			{
				// find symbol in current symbol table
				SymbolTable* table = this->currentSymbolTable;
				int index = table->getSymbolIndex(ast->left->value);  

				if ( ast->right->type == NodeType::N_NUMBER_C || ast->right->type == NodeType::N_ID)
					outfile << "mov eax, ";

				compile(ast->right);

				outfile << endl;
				outfile << "mov DWORD [ebp-" << 4*(index+1)  << "], eax" << endl;
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
				SymbolTable* table = this->currentSymbolTable;
				int index = table->getSymbolIndex(ast->value);  
				if (index >= 0) {
					outfile << "DWORD [ebp-" << (4*index) + 4 << "]";
				} else {
					// check input params in current function

					Node* args = this->currentFunction->args;
					index = 0;
					bool inFunctionArgs = false;

					while(args) {
						if (args->value == ast->value) {
							inFunctionArgs = true;
							break;
						}
						args = args->args;
						index++;
					}
					
					if (inFunctionArgs){
						outfile << "DWORD [ebp+" << (4*index) + 8 << "]";
					} else {
						cout << "Unexpected variable \"" << ast->value << "\" in compile:N_ID" << endl;
						exit(1);
					}
				}
			}
			break;

		case NodeType::N_NUMBER_C:
			outfile << ast->value;
			break;

		case NodeType::N_FUNC:
			this->currentFunction = ast;

			compile(ast->right);

			// prologue
			outfile << "fn_" << ast->value << ":" << endl << endl;

			compile(ast->left);

			// epilogue
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
			{
				Node* temp = ast;
				int count = 0;
				while(temp->args) {
					count++;

					if ( temp->args->type == NodeType::N_NUMBER_C || temp->args->type == NodeType::N_ID) {
						outfile << "push ";
						this->compile(temp->args);
					} else {
						this->compile(temp->args);
						outfile << endl;
						outfile << "push eax";
					}

					outfile << endl;
					temp = temp->args;
				}

				outfile << "call fn_" << ast->value << endl;

				// free arguments from stack
				if (count > 0) {
					outfile << "add esp, " <<  count*4 << endl;
				}
			}
			break;

		case NodeType::N_PROG:
			this->currentFunction = ast;
			this->currentSymbolTable = ast->symbolTable;
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

				SymbolTable *temp = this->currentSymbolTable;
				this->currentSymbolTable = ast->symbolTable;

				outfile << "push ebp" << endl;
				outfile << "mov ebp, esp" << endl;
				outfile << "sub esp, " << 4 * this->currentSymbolTable->getSize() << endl;

				compile(ast->left);

				outfile << "leave" << endl;
				this->currentSymbolTable = temp;

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
		default:
			break;
		//*/

	}
}

