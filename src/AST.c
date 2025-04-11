#include <stdio.h>

#include "../defs/AST.h"


void indent(int n) {
	for (int i = 0; i < n; ++i) {
		printf("\t");
	}
}

void printInner(AST *ast, int indentation) {
	indent(indentation);
	switch (ast->type) {
		case NodeConstant: {
			printf("Constant(%d);\n", ast->is.constant.value);
		} break;

		case NodeBinary: {
			char *op;
			switch (ast->is.binary.op) {
				case OpAdd: op = "+"; break;
				case OpSub: op = "-"; break;
				case OpMul: op = "*"; break;
				case OpDiv: op = "/"; break;
				case OpEqual: op = "=="; break;
				case OpNotEqual: op = "!="; break;
			}
			printf("Binary(%s) {\n", op);
			printInner(ast->is.binary.left, indentation + 1);
			printInner(ast->is.binary.right, indentation + 1);
			indent(indentation);
			printf("}\n");
		} break;
	}
}

int walkAST(AST *ast) {
	switch (ast->type) {
		case NodeConstant: {
			return ast->is.constant.value;
		}

		case NodeBinary: {
			switch (ast->is.binary.op) {
				case OpAdd: return walkAST(ast->is.binary.left) + walkAST(ast->is.binary.right);
				case OpSub: return walkAST(ast->is.binary.left) - walkAST(ast->is.binary.right);
				case OpMul: return walkAST(ast->is.binary.left) * walkAST(ast->is.binary.right);
				case OpDiv: return walkAST(ast->is.binary.left) / walkAST(ast->is.binary.right);

				case OpEqual: return walkAST(ast->is.binary.left) == walkAST(ast->is.binary.right);
				case OpNotEqual: return walkAST(ast->is.binary.left) != walkAST(ast->is.binary.right);
			}
		}

		default: return 0;
	}
}

void printAST(AST *ast) {
	printInner(ast, 0);
}

