#include "tree.h"

// =====================================================
// Token
// =====================================================

bool isOperation(TokenType type) {
    return type < TTYPE_BRACKET_OPEN;
}

int getPriority(TokenType type) {
    if (type == TTYPE_OPERATOR_MINUS ||
        type == TTYPE_OPERATOR_PLUS) {
        return 0;
    } else if (type == TTYPE_OPERATOR_DIV ||
               type == TTYPE_OPERATOR_MULT) {
        return 1;
    } else if (type == TTYPE_OPERATOR_UNARY_MINUS) {
        return 2;
    }
    return -1;
}

void tokenPrint(Token token) {
    if (isOperation(token.type)) {
        switch (token.type)
        {
        case TTYPE_OPERATOR_PLUS:
            printf("+");
            break;
        case TTYPE_OPERATOR_MINUS:
        case TTYPE_OPERATOR_UNARY_MINUS:
            printf("-");
            break;
        case TTYPE_OPERATOR_MULT:
            printf("*");
            break;
        case TTYPE_OPERATOR_DIV:
            printf("/");
            break;
        default:
            printf("???");
        }
    } else if (token.type == TTYPE_BRACKET_OPEN) {
        printf("(");
    } else if (token.type == TTYPE_BRACKET_CLOSE) {
        printf(")");
    } else if (token.type == TTYPE_NUMBER) {
        printf("%d", token.value);
    } else if (token.type == TTYPE_VARIABLE) {
        printf("%s", token.name);
    } else {
        printf("???");
    }
}

Token tokenMake(TokenType type) {
    Token token;
    token.type = type;
    return token;
}

// =====================================================
// TreeNode
// =====================================================

TreeNode *treeNodeCreate() {
    TreeNode *result = (TreeNode*)malloc(sizeof(TreeNode));
    result->left = NULL;
    result->right = NULL;
    return result;
}

TreeNode *treeNodeCreateToken(Token token) {
    TreeNode *result = treeNodeCreate();
    result->token = token;
    return result;
}

void treeNodeDestroy(TreeNode *node) {
    if (node != NULL) {
        free(node);
    }
}

void treeNodeDestroySubtree(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    treeNodeDestroySubtree(node->left);
    treeNodeDestroySubtree(node->right);
    treeNodeDestroy(node);
}

// %%% STATIC %%%%%%

static void treeNodePrintSubtreeMargin(TreeNode *node, int margin) {
    if (node == NULL) {
        return ;
    }
    for (int i = 0; i < margin; i++) {
        putchar(' ');
    }
    tokenPrint(node->token);
    putchar('\n');
    treeNodePrintSubtreeMargin(node->left, margin + TREE_PRINT_MARGIN);
    treeNodePrintSubtreeMargin(node->right, margin + TREE_PRINT_MARGIN);
}

// %%%%%%%%%%%%%%%%%%

void treeNodePrintSubtree(TreeNode *node) {
    treeNodePrintSubtreeMargin(node, 0);
}

// =====================================================
// Stack
// =====================================================

StackNode *stackNodeCreate() {
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    node->previous = NULL;
    node->value = NULL;
    return node;
}

void stackNodeDestroy(StackNode *node) {
    if (node != NULL) {
        free(node);
    }
}

Stack *stackCreate() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void stackDestroy(Stack *stack) {
    if (stack == NULL) {
        return ;
    }
    if (stack->top != NULL) {
        StackNode *current = stack->top;
        StackNode *prev = current->previous;
        while (prev != NULL) {
            stackNodeDestroy(current);
            current = prev;
            prev = current->previous;
        }
        stackNodeDestroy(current);
    }
    free(stack);
}

void stackDeepDestroy(Stack *stack) {
    if (stack == NULL) {
        return ;
    }
    if (stack->top != NULL) {
        StackNode *current = stack->top;
        StackNode *prev = current->previous;
        while (prev != NULL) {
            treeNodeDestroy(current->value);
            stackNodeDestroy(current);
            current = prev;
            prev = current->previous;
        }
        treeNodeDestroy(current->value);
        stackNodeDestroy(current);
    }
    free(stack);
}

void stackPush(Stack *stack, TreeNode *value) {
    if (stack == NULL) {
        return ;
    }
    if (stack->top == NULL) {
        stack->top = stackNodeCreate();
        stack->top->value = value;
    } else {
        StackNode *temp = stack->top;
        stack->top = stackNodeCreate();
        stack->top->value = value;
        stack->top->previous = temp;
    }
}

TreeNode *stackTop(Stack *stack) {
    if (stack == NULL) {
        return NULL;
    }
    if (stack->top == NULL) {
        return NULL;
    }
    return stack->top->value;
}

TreeNode *stackPop(Stack *stack) {
    TreeNode *temp = stackTop(stack);
    if (stack != NULL && stack->top != NULL) {
        StackNode *stemp = stack->top;
        stack->top = stack->top->previous;
        stackNodeDestroy(stemp);
    }
    return temp;
}

// =====================================================
// Parsing
// =====================================================

// %%% STATIC %%%%%%

static bool isDel(char c) {
    return c == ' ';
}

static bool isOper(char c) {
    return c == '-' || c == '+' || c == '/' || c == '*';
}

static bool isBracket(char c) {
    return c == '(' || c == ')';
}

static Token toOper(char c) {
    if (c == '-') {
        return tokenMake(TTYPE_OPERATOR_MINUS);
    } else if (c == '+') {
        return tokenMake(TTYPE_OPERATOR_PLUS);
    } else if (c == '/') {
        return tokenMake(TTYPE_OPERATOR_DIV);
    } else if (c == '*') {
        return tokenMake(TTYPE_OPERATOR_MULT);
    } else {
        return tokenMake(TTYPE_NULL);
    }
}

static Token toBracket(char c) {
    if (c == '(') {
        return tokenMake(TTYPE_BRACKET_OPEN);
    } else if (c == ')') {
        return tokenMake(TTYPE_BRACKET_CLOSE);
    } else {
        return tokenMake(TTYPE_NULL);
    }
}

// %%%%%%%%%%%%%%%%%%

int parse(TreeNode **result) {
    Stack *operationStack = stackCreate();
    Stack *resultStack = stackCreate();

    char buf[VARIABLE_MAX_LENGTH + 1];
    int ind = 0;
    bool isEnd = false;
    char c;

    typedef enum {
        PS_READY_FOR_TOKEN,
        PS_READING_NUMBER,
        PS_READING_VAR,
        PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR,
        PS_HAVE_READ_TOKEN_AND_ALREADY_GOT_CHAR,
        PS_ERROR
    } ParserState;

    ParserState state = PS_READY_FOR_TOKEN;

    Token token;
    TokenType previous = TTYPE_NULL;
    c = getchar();
    while (!isEnd) {
        ind = 0;
        if (state == PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR) {
            c = getchar();
        }
        state = PS_READY_FOR_TOKEN;
        while (state < PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR) {
            if (isEnd) {
                token = tokenMake(TTYPE_END);
                break;
            }
            if (state == PS_READY_FOR_TOKEN) {
                if (c == '-' && (previous == TTYPE_BRACKET_OPEN || previous == TTYPE_NULL || isOperation(previous))) {
                    token = tokenMake(TTYPE_OPERATOR_UNARY_MINUS);
                    state = PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR;
                    break;
                } else if (isOper(c)) {
                    token = toOper(c);
                    state = PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR;
                    break;
                } else if (isBracket(c)) {
                    token = toBracket(c);
                    state = PS_HAVE_READ_TOKEN_WITHOUT_GETTING_CHAR;
                    break;
                } else if (isdigit(c)) {
                    buf[ind] = c;
                    ind++;
                    state = PS_READING_NUMBER;
                } else if (isDel(c)) {
                    /* ... */
                } else if (c == '\n') {
                    token = tokenMake(TTYPE_END);
                    isEnd = true;
                    break;
                } else {
                    buf[ind] = c;
                    ind++;
                    state = PS_READING_VAR;
                }
            } else if (state == PS_READING_NUMBER) {
                if (isdigit(c)) {
                    buf[ind] = c;
                    if (ind == VARIABLE_MAX_LENGTH + 1) {
                        state = PS_ERROR;
                        break;
                    }
                    ind++;
                } else if (c == ')' || isOper(c) || isDel(c) || c == '\n') {
                    buf[ind] = '\0';
                    token = tokenMake(TTYPE_NUMBER);
                    token.value = atoi(buf);
                    state = PS_HAVE_READ_TOKEN_AND_ALREADY_GOT_CHAR;
                    if (c == '\n') {
                        isEnd = true;
                    }
                    break;
                } else {
                    state = PS_ERROR;
                    break;
                }
            } else if (state == PS_READING_VAR) {
                if (isOper(c) || isDel(c) || c == ')' || c == '\n') {
                    buf[ind] = '\0';
                    token = tokenMake(TTYPE_VARIABLE);
                    strcpy(token.name, buf);
                    state = PS_HAVE_READ_TOKEN_AND_ALREADY_GOT_CHAR;
                    if (c == '\n') {
                        isEnd = true;
                    }
                    break;
                } else {
                    buf[ind] = c;
                    if (ind == VARIABLE_MAX_LENGTH + 1) {
                        state = PS_ERROR;
                        break;
                    }
                    ind++;
                }
            }
            c = getchar();
        }
        if (state == PS_ERROR) {
            stackDeepDestroy(operationStack);
            stackDeepDestroy(resultStack);
            return PARSE_STATUS_FAIL;
        }
        // Сортировочная станция Дейкстры.
        if (isOperation(token.type)) {
            TreeNode *node;
            node = stackTop(operationStack);
            while (node != NULL && isOperation(node->token.type) && getPriority(token.type) <= getPriority(node->token.type)) {
                stackPop(operationStack);
                if (node->token.type == TTYPE_OPERATOR_UNARY_MINUS) {
                    node->left = stackPop(resultStack);
                } else {
                    node->right = stackPop(resultStack);
                    node->left = stackPop(resultStack);
                }
                stackPush(resultStack, node);
                node = stackTop(operationStack);
            }
            stackPush(operationStack, treeNodeCreateToken(token));
        } else if (token.type == TTYPE_NUMBER || token.type == TTYPE_VARIABLE) {
            stackPush(resultStack, treeNodeCreateToken(token));
        } else if (token.type == TTYPE_BRACKET_OPEN) {
            stackPush(operationStack, treeNodeCreateToken(token));
        } else if (token.type == TTYPE_BRACKET_CLOSE) {
            TreeNode *node;
            node = stackPop(operationStack);
            while (node != NULL && node->token.type != TTYPE_BRACKET_OPEN) {
                node->right = stackPop(resultStack);
                node->left = stackPop(resultStack);
                stackPush(resultStack, node);
                node = stackPop(operationStack);
            }
            if (node == NULL) {
                stackDeepDestroy(operationStack);
                stackDeepDestroy(resultStack);
                return PARSE_STATUS_FAIL;
            }
            treeNodeDestroy(node);
        }
        //
        previous = token.type;
    }
    TreeNode *node;
    node = stackPop(operationStack);
    while (node != NULL) {
        if (node->token.type == TTYPE_BRACKET_OPEN) {
            stackDeepDestroy(operationStack);
            stackDeepDestroy(resultStack);
            return PARSE_STATUS_FAIL;
        }
        if (node->token.type == TTYPE_OPERATOR_UNARY_MINUS) {
            node->left = stackPop(resultStack);
        } else {
            node->right = stackPop(resultStack);
            node->left = stackPop(resultStack);
        }
        stackPush(resultStack, node);
        node = stackPop(operationStack);
    }
    *result = stackTop(resultStack);
    stackDestroy(operationStack);
    stackDestroy(resultStack);
    return PARSE_STATUS_OK;
}

// =====================================================
// Decoding
// =====================================================

void printInfix(TreeNode *node, bool brackets) {
    if (node == NULL) {
        return ;
    }
    if (brackets) {
        putchar('(');
    }
    if (node->token.type == TTYPE_VARIABLE ||
        node->token.type == TTYPE_NUMBER) {
        tokenPrint(node->token);
    } else if (node->token.type == TTYPE_OPERATOR_UNARY_MINUS) {
        putchar('-');
        if (isOperation(node->left->token.type)) {
            printInfix(node->left, true);
        } else {
            printInfix(node->left, false);
        }
    } else if (isOperation(node->token.type)) {
        if (isOperation(node->left->token.type) && getPriority(node->token.type) > getPriority(node->left->token.type)) {
            printInfix(node->left, true);
        } else {
            printInfix(node->left, false);
        }
        putchar(' ');
        tokenPrint(node->token);
        putchar(' ');
        if (isOperation(node->right->token.type) && getPriority(node->token.type) > getPriority(node->right->token.type)) {
            printInfix(node->right, true);
        } else {
            printInfix(node->right, false);
        }
    }
    if (brackets) {
        putchar(')');
    }
}

// =====================================================
// Special function
// =====================================================

void simplify(TreeNode *node) {
    if (node == NULL) {
        return ;
    }
    simplify(node->left);
    simplify(node->right);
    if (  node->token.type == TTYPE_OPERATOR_MULT &&
          ((node->left->token.type == TTYPE_NUMBER &&
          node->left->token.value == 0) ||
          (node->right->token.type == TTYPE_NUMBER &&
          node->right->token.value == 0))) {
        treeNodeDestroySubtree(node->left);
        treeNodeDestroySubtree(node->right);
        node->token = tokenMake(TTYPE_NUMBER);
        node->token.value = 0;
        node->left = NULL;
        node->right = NULL;
    }
}
