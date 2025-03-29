#ifndef PARSER_H
#define PARSER_H

typedef enum {
	ParseOK,
	ParseErr,
} ParseStatus;

typedef struct {
	ParseStatus status;
	const char *message;
} ParseResult;

typedef enum {
	OpAdd, OpSub, OpMul,
} OpType;

typedef struct {
	int left, right;
	OpType op;
} AST;

ParseResult parse(const char **program, AST *ast);

#endif /* PARSER_H */
