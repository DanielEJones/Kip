#ifndef AST_H
#define AST_H

// -------------------------------------------------------------------------
// Types that the define the AST
//

typedef enum {
	OpAdd, OpSub, OpMul, OpDiv,
} OpType;

typedef enum {
	NodeConstant, NodeBinary
} NodeType;

typedef struct ast_node {
	NodeType type;

	union {

		struct {
			int value;
		} constant;

		struct {
			OpType op;
			struct ast_node *left, *right;
		} binary;

	} is;

} AST;

// -------------------------------------------------------------------------
// Functions for interacting with the AST
//

void printAST(AST *ast);
int walkAST(AST *ast);

#endif /* AST_H */
