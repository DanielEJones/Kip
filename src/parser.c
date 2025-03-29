#include <stdlib.h>

#include "../defs/parser.h"


#define expect(PARSE) \
do { \
	ParseResult res = PARSE;\
	if (res.status != ParseOK) return res; \
} while(0);\

#define PARSE_ERR(MSG) (ParseResult) { .status = ParseErr, .message = MSG }
#define PARSE_OK() (ParseResult) { .status = ParseOK, .message = NULL }

void skipWhitespace(const char **stream) {
	while (**stream == ' ') (*stream)++;
}

ParseResult parseNumber(const char **stream, int *dest) {
	const char *start = *stream;
	const char *end = *stream;

	if ('0' > *end || *end > '9') return PARSE_ERR("Expected one or more digits.");

	while ('0' <= *end && *end <= '9') end++;

	int len = end - start;

	int n = 0, p = 1;
	for (int i = 1; i <= len; ++i) {
		n += (int)(*(end - i) - '0') * p;
		p *= 10;
	}

	*dest = n;
	*stream += len;

	return PARSE_OK();
}

ParseResult parseOperator(const char **stream, OpType *op) {
	switch (**stream) {
		case '+': *op = OpAdd; (*stream)++; return PARSE_OK();
		case '-': *op = OpSub; (*stream)++; return PARSE_OK();
		case '*': *op = OpMul; (*stream)++; return PARSE_OK();
		default: return PARSE_ERR("Unrecognised Operator.");
	}
}

ParseResult parseExpression(const char **stream, AST *ast) {
	// expr = num op num
	expect(parseNumber(stream, &ast->left));
	skipWhitespace(stream);
	expect(parseOperator(stream, &ast->op));
	skipWhitespace(stream);
	expect(parseNumber(stream, &ast->right));
	return PARSE_OK();
}

ParseResult parse(const char **program, AST *ast) {
	return parseExpression(program, ast);
}

