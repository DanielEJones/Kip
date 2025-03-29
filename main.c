#include <stdlib.h>
#include <stdio.h>

#include "defs/parser.h"


int main(int argc, char *argv[]) {
	const char *program = argv[1];

	AST ast;
	ParseResult pres = parse(&program, &ast);
	if (pres.status != ParseOK) {
		fprintf(stderr, "Parsing Failed:\n");
		fprintf(stderr, "  | %s\n  | ", argv[1]);
		for (int i = 0; i < (program - argv[1]); ++i) {
			fprintf(stderr, "~");
		}
		fprintf(stderr, "^ %s\n", pres.message);
		exit(EXIT_FAILURE);
	}

	int ores;
	switch (ast.op) {
		case OpAdd: {ores = ast.left + ast.right; break;}
		case OpSub: {ores = ast.left - ast.right; break;}
		case OpMul: {ores = ast.left * ast.right; break;}
	}

	printf("%d\n", ores);
	exit(EXIT_SUCCESS);
}
