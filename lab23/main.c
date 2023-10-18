#include "tree.h"

const char * MENU_TEXT = "1. Напечатать дерево.\n"
                         "2. Добавить вершину в дерево.\n"
                         "3. Удалить вершину.\n"
                         "4. Найти количество вершин, степень которых совпадает с их значением.\n"
                         "5. Завершить программу.\n";

void insertRoutine(Tree *tree) {
    int value1, value2;
    if (tree->root == NULL) {
        printf("Введите значение элемента для вставки: ");
        scanf("%d", &value1);
        if (!treeInsert(tree, value1, 0)) {
            printf("Не удалось вставить вершину.\n");
            return ;
        }
    } else {
        printf("Введите значения элемента для вставки и родителя: ");
        scanf("%d %d", &value1, &value2);
        if (!treeInsert(tree, value1, value2)) {
            printf("Не удалось вставить вершину.\n");
            return ;
        }
    }
    printf("Успешно.\n");
}

void eraseRoutine(Tree *tree) {
    int value;
    printf("Введите значение элемента для удаления: ");
    scanf("%d", &value);
    if (!treeEraseNode(tree, value)) {
        printf("Не удалось удалить вершину.\n");
    } else {
        printf("Успешно.\n");
    }
}

int main() {
    Tree *tree = treeCreate();

    int choice;

    while (true) {
        printf("%s", MENU_TEXT);
        scanf("%d", &choice);
        printf("\n");
        if (choice == 1) {
            treePrint(tree);
        } else if (choice == 2) {
            insertRoutine(tree);
        } else if (choice == 3) {
            eraseRoutine(tree);
        } else if (choice == 4) {
            printf("%d", treeCountFunc(tree));
        } else if (choice == 5) {
            break;
        } else {
            printf("Неизвестная команда.\n");
        }
        printf("\n");
    }

    treeDestroy(tree);
}
