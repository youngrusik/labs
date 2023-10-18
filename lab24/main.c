#include "tree.h"

int main() {
    TreeNode *node;
    if (parse(&node) == PARSE_STATUS_FAIL) {
        printf("Некорректное выражение!\n");
        exit(EXIT_FAILURE);
    }
    printf("Соответствующее дерево выражения:\n");
    treeNodePrintSubtree(node);
    simplify(node);
    printf("Соответствующее дерево выражения после преобразования:\n");
    treeNodePrintSubtree(node);
    printf("Само выражение после преобразования:\n");
    printInfix(node, false);
    putchar('\n');
    treeNodeDestroySubtree(node);
}
