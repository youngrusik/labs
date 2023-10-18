#ifndef _TREE_H_
#define _TREE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_PRINT_MARGIN 4

typedef struct _TreeNode {
    int value, degree;
    struct _TreeNode *child, *rightBrother;
} TreeNode;

typedef struct {
    TreeNode *root;
} Tree;

// TreeNode

TreeNode *treeNodeCreate(int value);
void treeNodeDestroy(TreeNode *node);
void treeNodeDestroySubtree(TreeNode *node);
TreeNode* treeNodeSubtreeSearch(TreeNode *node, int value);

// Tree

Tree *treeCreate();
void treeDestroy(Tree *tree);
TreeNode *treeSearch(Tree *tree, int value);
void treePrint(Tree *tree);
bool treeInsert(Tree *tree, int value, int parentValue);
bool treeEraseNode(Tree *tree, int value);
int treeCountFunc(Tree *tree);

#endif
