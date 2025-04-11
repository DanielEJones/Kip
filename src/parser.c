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
// AST utilities
//

AST *newNode(NodeType type) {
	AST *node = malloc(sizeof(AST));
	node->type = type;
	return node;
}

AST *newConstant(ValueType type, int value) {
	AST *node = newNode(NodeConstant);
	node->is.constant.t = type;
	node->is.constant.value = value;
	return node;
}

AST *newBinary(OpType type, AST *left, AST *right) {
	AST *node = newNode(NodeBinary);
	node->is.binary.op = type;
	node->is.binary.left = left;
	node->is.binary.right = right;
	return node;
}

// -------------------------------------------------------------------------
// Parse Functions
//

/*
 * program = cmp
 * cmp = expr [== expr]
 * expr = term [(+ | -) expr]
 * term = factor [(* | /) term]
 * factor = (expr) | num
 */

AST *parseExpression(Parser *parser, ParseResult *result);

AST *parseFactor(Parser *parser, ParseResult *result) {
	if (expect(parser, TokenInt)) {
		return newConstant(
			ValueInt,
			make_number(advance(parser))
		);
	} else if (expect(parser, TokenLParen)) {
		advance(parser);
		AST *node = parseExpression(parser, result);
		advance(parser);
		return node;
	} else {
		result->status = ParseErr;
		result->message = "Expeted a number or nested expression.";
		return newConstant(ValueInt, 99);
	}
}

AST *parseTerm(Parser *parser, ParseResult *result) {
	AST *node = parseFactor(parser, result);
	while (expect(parser, TokenStar) || expect(parser, TokenSlash)) {
		OpType op = (current(parser)->type == TokenStar) ? OpMul : OpDiv;
		advance(parser);
		AST *new = newBinary(op, node, parseFactor(parser, result));
		node = new;
	}
	return node;
}

AST *parseExpression(Parser *parser, ParseResult *result) {
	AST *node = parseTerm(parser, result);
	while (expect(parser, TokenPlus) || expect(parser, TokenDash)) {
		OpType op = (current(parser)->type == TokenPlus) ? OpAdd : OpSub;
		advance(parser);
		AST *new = newBinary(op, node, parseTerm(parser, result));
		node = new;
	}
	return node;
}

AST *parseComparison(Parser *parser, ParseResult *result) {
	AST *node = parseExpression(parser, result);
	while (expect(parser, TokenDoubleEqual) || expect(parser, TokenBangEqual)) {
		OpType op = (current(parser)->type == TokenDoubleEqual ? OpEqual : OpNotEqual);
		advance(parser);
		AST *new = newBinary(op, node, parseExpression(parser, result));
		node = new;
	}
	return node;
}

AST *parse(Parser *parser, ParseResult *result) {
	return parseComparison(parser, result);
}

void initParser(Parser *parser, TokenList *tokens) {
	parser->tokens = tokens;
}

