#ifndef AST_H
#define AST_H

// -------------------------------------------------------------------------
// Types that the define the AST
//

typedef enum {
	OpAdd, OpSub, OpMul, OpDiv,
	OpEqual, OpNotEqual,
} OpType;

typedef enum {
	NodeConstant, NodeBinary
} NodeType;

typedef enum {
	ValueInt, ValueBool,
} ValueType;

typedef struct ast_node {
	NodeType type;

	union {

		struct {
			ValueType t;
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
