#include <stdlib.h>
#include <stdio.h>


typedef enum {
	ParseOK,
	ParseErr,
} ParseResult;

typedef struct {
	int left, right;
} AST;

ParseResult parseNumber(const char **statement, int *dest) {
	const char *start = *statement;
	const char *end = *statement;

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

ParseResult parse(const char **statement, AST *ast) {
	parseNumber(statement, &ast->left);
	*statement += 3;
	parseNumber(statement, &ast->right);
	return ParseOK;
}

int main(int argc, char *argv[]) {
	const char *program = argv[1];

	AST ast;
	parse(&program, &ast);

	printf("%d\n", ast.left + ast.right);
	exit(EXIT_SUCCESS);
}
