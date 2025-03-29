#include <stdlib.h>
#include <stdio.h>


typedef enum {
	ParseOK,
	ParseErr,
} ParseResult;

typedef enum {
	OpAdd, OpSub, OpMul,
} OpType;

typedef struct {
	int left, right;
	OpType op;
} AST;

#define require(PARSE) if (PARSE == ParseErr) return ParseErr;

ParseResult parseNumber(const char **statement, int *dest) {
	const char *start = *statement;
	const char *end = *statement;

	if ('0' > *end || *end > '9') return ParseErr;

	while ('0' <= *end && *end <= '9') end++;

	int len = end - start;

	int n = 0, p = 1;
	for (int i = 1; i <= len; ++i) {
		n += (int)(*(end - i) - '0') * p;
		p *= 10;
	}

	*dest = n;
	*statement += len;

	return ParseOK;
}

ParseResult parseOperator(const char **statement, OpType *op) {
	switch (**statement) {
		case '+': *op = OpAdd; *statement += 1; return ParseOK;
		case '-': *op = OpSub; *statement += 1; return ParseOK;
		case '*': *op = OpMul; *statement += 1; return ParseOK;
		default: return ParseErr;
	}
}

ParseResult parse(const char **statement, AST *ast) {
	require(parseNumber(statement, &ast->left));
	(*statement)++;
	require(parseOperator(statement, &ast->op));
	(*statement)++;
	require(parseNumber(statement, &ast->right));
	return ParseOK;
}

int main(int argc, char *argv[]) {
	const char *program = argv[1];

	AST ast;
	ParseResult pres = parse(&program, &ast);
	if (pres != ParseOK) {
		fprintf(stderr, "Parsing Failed.\n");
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
