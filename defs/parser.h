#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"


// -------------------------------------------------------------------------
// Types used to indicate and bubble up return values of parse functions
//

typedef enum {
	ParseOK,
	ParseErr,
} ParseStatus;

typedef struct {
	ParseStatus status;
	const char *message;
} ParseResult;

// -------------------------------------------------------------------------
// Types that the define the AST
//

typedef enum {
	OpAdd, OpSub, OpMul,
} OpType;

typedef struct {
	Token *left, *right;
	Token *op;
} AST;


// -------------------------------------------------------------------------
// Things that parse a program
//

typedef struct {
	AST *ast;
	TokenList *tokens;
	size_t current;
} Parser;

void initParser(Parser *parser, TokenList *tokens);
ParseResult parse(Parser *parser);

#endif /* PARSER_H */
