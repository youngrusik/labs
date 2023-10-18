#include "tree.h"

// TreeNode

TreeNode *treeNodeCreate(int value) {
    TreeNode *result = (TreeNode*)malloc(sizeof(TreeNode));
    if (result == NULL) {
        return NULL;
    }
    result->value = value;
    result->degree = 0;
    result->child = NULL;
    result->rightBrother = NULL;
    return result;
}

void treeNodeDestroy(TreeNode *node) {
    if (node != NULL) {
        free(node);
    }
}

void treeNodeDestroySubtree(TreeNode *node) {
    if (node == NULL) {
        return ;
    }
    if (node->child != NULL) {
        TreeNode *currentChild = node->child;
        TreeNode *nextChild = currentChild->rightBrother;
        while (currentChild != NULL) {
            treeNodeDestroySubtree(currentChild);
            currentChild = nextChild;
            if (currentChild != NULL) {
                nextChild = currentChild->rightBrother;
            }
        }
    }
    treeNodeDestroy(node);
}

TreeNode* treeNodeSubtreeSearch(TreeNode *node, int value) {
    if (node == NULL) {
        return NULL;
    }
    if (node->value == value) {
        return node;
    }
    TreeNode *result;
    if ((result = treeNodeSubtreeSearch(node->rightBrother, value)) != NULL) {
        return result;
    } else if ((result = treeNodeSubtreeSearch(node->child, value)) != NULL) {
        return result;
    }
    return NULL;
}

// Tree

Tree *treeCreate() {
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

void treeDestroy(Tree *tree) {
    if (tree != NULL) {
        treeNodeDestroySubtree(tree->root);
        free(tree);
    }
}

TreeNode *treeSearch(Tree *tree, int value) {
    return treeNodeSubtreeSearch(tree->root, value);
}

static void treeNodePrint(TreeNode *node, int margin) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < margin; i++) {
        printf(" ");
    }
    printf("%d\n", node->value);
    if (node->child != NULL) {
        TreeNode *currentChild = node->child;
        while (currentChild != NULL) {
            treeNodePrint(currentChild, margin + TREE_PRINT_MARGIN);
            currentChild = currentChild->rightBrother;
        }
    }
}

void treePrint(Tree *tree) {
    treeNodePrint(tree->root, 0);
}

static void treeInsertNode(TreeNode *parent, int value) {
    TreeNode *newNode = treeNodeCreate(value);
    if (parent->child == NULL) {
        parent->child = newNode;
        return ;
    }
    TreeNode *currentChild = parent->child;
    while (currentChild->rightBrother != NULL) {
        currentChild = currentChild->rightBrother;
    }
    currentChild->rightBrother = newNode;
}

bool treeInsert(Tree *tree, int value, int parentValue) {
    if (tree == NULL) {
        return false;
    }
    if (tree->root == NULL) {
        tree->root = treeNodeCreate(value);
        return true;
    }
    TreeNode *parent = treeSearch(tree, parentValue);
    if (parent == NULL) {
        return false;
    }
    treeInsertNode(parent, value);
    parent->degree++;
    return true;
}

static bool treeNodeSubtreeErase(TreeNode *node, int value) {
    if (node == NULL) {
        return false;
    }
    if (node->child == NULL) {
        return false;
    }
    TreeNode *previousChild = NULL;
    TreeNode *currentChild = node->child;
    while (currentChild != NULL) {
        if (treeNodeSubtreeErase(currentChild, value)) {
            return true;
        }
        if (value == currentChild->value) {
            if (previousChild == NULL) {
                node->child = currentChild->rightBrother;
                treeNodeDestroySubtree(currentChild);
            } else {
                previousChild->rightBrother = currentChild->rightBrother;
                treeNodeDestroySubtree(currentChild);
            }
            node->degree--;
            return true;
        }
        previousChild = currentChild;
        currentChild = currentChild->rightBrother;
    }
    return false;
}

bool treeEraseNode(Tree *tree, int value) {
    if (tree == NULL) {
        return false;
    }
    if (tree->root == NULL) {
        return false;
    }
    if (tree->root->value == value) {
        treeNodeDestroySubtree(tree->root);
        tree->root = NULL;
        return true;
    }
    return treeNodeSubtreeErase(tree->root, value);
}

static int treeNodeCountFunc(TreeNode *node) {
    int result = 0;
    if (node != NULL) {
        TreeNode *currentChild = node->child;
        while (currentChild != NULL) {
            result += treeNodeCountFunc(currentChild);
            currentChild = currentChild->rightBrother;
        }
        if (node->value == node->degree) {
            result++;
        }
    }
    return result;
}

int treeCountFunc(Tree *tree) {
    if (tree == NULL) {
        return 0;
    }
    return treeNodeCountFunc(tree->root);
}
