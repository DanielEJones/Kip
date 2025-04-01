#include "../defs/parser.h"

#include <stdlib.h>


// -------------------------------------------------------------------------
// Parsing Utilities
//

Token *current(Parser *parser) {
	if (parser->current < parser->tokens->count) {
		return &parser->tokens->tokens[parser->current];
	}
	return &parser->tokens->tokens[parser->tokens->count];
}

Token *advance(Parser *parser) {
	if (current(parser)->type == TokenEOF) {
		return current(parser);
	}
	return &parser->tokens->tokens[parser->current++];
}

int expect(Parser *parser, TokenType type) {
	if (current(parser)->type == TokenSpace) {
		advance(parser);
	}
	return current(parser)->type == type;
}

// -------------------------------------------------------------------------
// Parse Functions
//

/*
 * program = expr
 * expr = term [(+ | -) expr]
 * term = factor [(* | /) term]
 * factor = (expr) | num
 */

ParseResult parseExpression(Parser *parser) {
	if (!expect(parser, TokenInt)) {
		return (ParseResult) { .status = ParseErr, .message = "Expected an Integer." };
	}

	parser->ast->left = advance(parser);

	if (!(expect(parser, TokenPlus) || expect(parser, TokenDash)
		|| expect(parser, TokenStar) || expect(parser, TokenSlash)
	)) {
		return (ParseResult) { .status = ParseErr, .message = "Expected an Operator." };
	}

	parser->ast->op = advance(parser);

	if (!expect(parser, TokenInt)) {
		return (ParseResult) { .status = ParseErr, .message = "Expected an Integer." };
	}

	parser->ast->right = advance(parser);

	return (ParseResult) { .status = ParseOK };
}

ParseResult parse(Parser *parser) {
	return parseExpression(parser);
}

void initParser(Parser *parser, TokenList *tokens) {
	parser->tokens = tokens;
	parser->ast = (AST *) malloc(sizeof(AST));
}

