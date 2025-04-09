#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AST.h"


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
// Things that parse a program
//

typedef struct {
	TokenList *tokens;
	size_t current;
} Parser;

void initParser(Parser *parser, TokenList *tokens);
AST *parse(Parser *parser, ParseResult *result);

#endif /* PARSER_H */
