#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>


// -------------------------------------------------------------------------
// Types that the define a Token
//

typedef enum {
	TokenInt,
	TokenPlus, TokenDash, TokenStar, TokenSlash,
	TokenLParen, TokenRParen,
	TokenSpace, TokenNewline, TokenComment,
	TokenEOF, TokenError,
} TokenType;

typedef struct {
	TokenType type;
	const char *start;
	size_t len;
} Token;

// -------------------------------------------------------------------------
// Dynamic Token Lists to be used during file parsing
//

typedef struct {
	size_t cap, count;
	Token *tokens;
} TokenList;

void appendToken(TokenList *list, Token token);

// -------------------------------------------------------------------------
// Functions to convert a file into a list of tokens
//

typedef struct {
	const char *source;
	TokenList tokens;
} Lexer;

void initLexer(Lexer *lexer, const char *source);
void lex(Lexer *lexer);

#endif /* LEXER_H */
