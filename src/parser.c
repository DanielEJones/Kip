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

int make_number(Token *token) {
	int n = 0, p = 1;
	for (int i = token->len - 1; i >= 0; --i) {
		n += p * (*(token->start + i) - '0');
		p *= 10;
	}
	return n;
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

AST *parseExpression(Parser *parser, ParseResult *result);

AST *parseFactor(Parser *parser, ParseResult *result) {
	if (expect(parser, TokenInt)) {
		AST *node = malloc(sizeof(AST));
		node->type = NodeConstant;
		node->is.constant.value = make_number(current(parser));
		advance(parser);
		return node;
	} else if (expect(parser, TokenLParen)) {
		advance(parser);
		AST *node = parseExpression(parser, result);
		advance(parser);
		return node;
	} else {
		result->status = ParseErr;
		result->message = "Expeted a number or nested expression.";
		AST *node = malloc(sizeof(AST));
		node->type = NodeConstant;
		node->is.constant.value = 99;
		return node;
	}
}

AST *parseTerm(Parser *parser, ParseResult *result) {
	AST *node = parseFactor(parser, result);
	while (expect(parser, TokenStar) || expect(parser, TokenSlash)) {
		AST *new = malloc(sizeof(AST));
		OpType op = (current(parser)->type == TokenStar) ? OpMul : OpDiv;
		advance(parser);
		new->type = NodeBinary;
		new->is.binary.op = op;
		new->is.binary.left = node;
		new->is.binary.right = parseFactor(parser, result);
		node = new;
	}
	return node;
}

AST *parseExpression(Parser *parser, ParseResult *result) {
	AST *node = parseTerm(parser, result);
	while (expect(parser, TokenPlus) || expect(parser, TokenDash)) {
		AST *new = malloc(sizeof(AST));
		OpType op = (current(parser)->type == TokenPlus) ? OpAdd : OpSub;
		advance(parser);
		new->type = NodeBinary;
		new->is.binary.op = op;
		new->is.binary.left = node;
		new->is.binary.right = parseTerm(parser, result);
		node = new;
	}
	return node;
}

AST *parse(Parser *parser, ParseResult *result) {
	return parseExpression(parser, result);
}

void initParser(Parser *parser, TokenList *tokens) {
	parser->tokens = tokens;
}

