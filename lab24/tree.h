#ifndef _TREE_H_
#define _TREE_H_

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define VARIABLE_MAX_LENGTH 20
#define TREE_PRINT_MARGIN 4

// =====================================================
// Token
// =====================================================

typedef enum {
    TTYPE_OPERATOR_MINUS,
    TTYPE_OPERATOR_PLUS,
    TTYPE_OPERATOR_MULT,
    TTYPE_OPERATOR_DIV,
    TTYPE_OPERATOR_UNARY_MINUS,
    TTYPE_BRACKET_OPEN,
    TTYPE_BRACKET_CLOSE,
    TTYPE_NUMBER,
    TTYPE_VARIABLE,
    TTYPE_NULL,
    TTYPE_END
} TokenType;

bool isOperation(TokenType type);
int getPriority(TokenType type);

typedef struct {
    TokenType type;
    union {
        int value; // only for numbers
        char name[VARIABLE_MAX_LENGTH + 1]; // only for variables
    };
} Token;

void tokenPrint(Token token);
Token tokenMake(TokenType type);

// =====================================================
// TreeNode
// =====================================================

typedef struct _TreeNode {
    Token token;
    struct _TreeNode *left, *right;
} TreeNode;

TreeNode *treeNodeCreate();
TreeNode *treeNodeCreateToken(Token token);
void treeNodeDestroy(TreeNode *node);
void treeNodeDestroySubtree(TreeNode *node);
void treeNodePrintSubtree(TreeNode *node);

// =====================================================
// Stack
// =====================================================

typedef struct _StackNode {
    TreeNode *value;
    struct _StackNode *previous;
} StackNode;

typedef struct {
    StackNode *top;
} Stack;

StackNode *stackNodeCreate();
void stackNodeDestroy(StackNode *node);

Stack *stackCreate();
void stackDestroy(Stack *stack);
void stackDeepDestroy(Stack *stack);
void stackPush(Stack *stack, TreeNode *value);
TreeNode *stackTop(Stack *stack);
TreeNode *stackPop(Stack *stack);

// =====================================================
// Parsing
// =====================================================

#define PARSE_STATUS_OK 0
#define PARSE_STATUS_FAIL -1

int parse(TreeNode **result);

// =====================================================
// Decoding
// =====================================================

void printInfix(TreeNode *node, bool brackets);

// =====================================================
// Special function
// =====================================================

void simplify(TreeNode *node);

#endif
