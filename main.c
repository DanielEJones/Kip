#include <stdlib.h>
#include <stdio.h>

#include "defs/AST.h"
#include "defs/lexer.h"
#include "defs/parser.h"


void printTokenInner(Token *token) {
	for (int i = 0; i < token->len; ++i) {
		switch (*(token->start + i)) {
			case '\n': printf("\\n"); break;
			case '\t': printf("\\t"); break;
			default: printf("%c", *(token->start + i)); break;
		}
	}
}

void printToken(Token *token) {
	switch (token->type) {
		case TokenInt: printf("<Int: "); printTokenInner(token); printf(">"); break;
		case TokenComment: printf("<Comment: "); printTokenInner(token); printf(">"); break;
		case TokenPlus: printf("<Plus>"); break;
		case TokenDash: printf("<Dash>"); break;
		case TokenStar: printf("<Star>"); break;
		case TokenSlash: printf("<Slash>"); break;
		case TokenSpace: printf("<Space>"); break;
		case TokenLParen: printf("<(>"); break;
		case TokenRParen: printf("<)>"); break;
		case TokenEOF: printf("<EOF>"); break;
		case TokenError: printf("<Error: %s>", token->start); break;
		default: printf("A Token"); break;
	}
}

int main(int argc, char *argv[]) {
	const char *program = argv[1];

	Lexer lexer;
	initLexer(&lexer, program);

	lex(&lexer);

	Parser parser;
	initParser(&parser, &lexer.tokens);

	ParseResult pres = { .status = ParseOK };
	AST *ast = parse(&parser, &pres);

	if (pres.status != ParseOK) {
		fprintf(stderr, "Parsing Failed:\n");
		fprintf(stderr, "  | %s\n  | ", argv[1]);
		for (int i = 0; i < parser.current; ++i) {
			for (int j = 0; j < parser.tokens->tokens[i].len; ++j) {
				fprintf(stderr, "~");
			}
		}
		fprintf(stderr, "^ %s\n", pres.message);
		exit(EXIT_FAILURE);
	}

	printAST(ast);
	printf("%d\n", walkAST(ast));

	exit(EXIT_SUCCESS);
}
