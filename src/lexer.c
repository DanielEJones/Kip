#include "../defs/lexer.h"

#include <stdlib.h>


void appendToken(TokenList *list, Token token) {
	if (list->count + 1 >= list->cap) {
		list->cap = (list->cap < 8) ? 8 : 2 * list->cap;
		list->tokens = realloc(list->tokens, list->cap * sizeof(Token));
	}

	list->tokens[list->count++] = token;
}

void initLexer(Lexer *lexer, const char *source) {
	lexer->source = source;
}

void lexNumber(Lexer *lexer) {
	const char *end = lexer->source;
	while ('0' <= *end && *end <= '9') end++;
	size_t len = end - lexer->source;
	appendToken(&lexer->tokens, (Token) {
		.type = TokenInt,
		.start = lexer->source,
		.len = len,
	});
	lexer->source += len;
}

void lexSpaces(Lexer *lexer) {
	const char *end = lexer->source;
	while (*end == ' ' || *end == '\t') end++;
	size_t len = end - lexer->source;
	appendToken(&lexer->tokens, (Token) {
		.type = TokenSpace,
		.start = lexer->source,
		.len = len,
	});
	lexer->source += len;
}

void lexComment(Lexer *lexer) {
	const char *end = lexer->source;
	while (*end != '\0' && *end != '\n') end++;
	size_t len = end - lexer->source;
	appendToken(&lexer->tokens, (Token) {
		.type = TokenComment,
		.start = lexer->source,
		.len = len,
	});
	lexer->source += len;
}

void lex(Lexer *lexer) {
	for (;;) {
		char current = *lexer->source;

		if ('0' <= current && current <= '9') {
			lexNumber(lexer);
			continue;
		}

		if (current == ' ' || current == '\t') {
			lexSpaces(lexer);
			continue;
		}

		if (current == '/' && *(lexer->source + 1) == '/') {
			lexComment(lexer);
			continue;
		}

		Token t = {
			.start = lexer->source,
			.len = 1,
		};

		switch (*lexer->source) {
			case '+': t.type = TokenPlus; break;
			case '-': t.type = TokenDash; break;
			case '*': t.type = TokenStar; break;
			case '/': t.type = TokenSlash; break;
			case '(': t.type = TokenLParen; break;
			case ')': t.type = TokenRParen; break;
			case '\n': t.type = TokenNewline; break;

			case '\0': {
				appendToken(&lexer->tokens, (Token) { .type = TokenEOF, .start = "EOF", .len = 3 });
				return;
			}

			default: {
				t.type = TokenError;
				t.start = "Unrecognised token";
				t.len = 25;
			}
		}

		appendToken(&lexer->tokens, t);
		lexer->source++;
	}
	appendToken(&lexer->tokens, (Token) { .type = TokenEOF, .start = "EOF", .len = 3 });
}
