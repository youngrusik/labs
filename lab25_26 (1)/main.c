#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main() 
{
    Stack stack;
    stackInit(&stack);

    int choice;
    while (1) {
    	printf("1. Добавить элемент стека?\n");
        printf("2. Удалить последний элемент?\n");
        printf("3. Распечатать стек?\n");
        printf("4. Отсортировать стек?\n");
        printf("5. Закончить работу программы?\n");

        printf("Введите номер команды: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Вы выбрали команду: Добавить элемент стека\n");
                printf("Введите значение элемента: ");
                int data;
                scanf("%d", &data);
        		stackPush(&stack, data);
                break;
            case 2:
                printf("Вы выбрали команду: Удалить последний элемент\n");
        	    stackPop(&stack);
                break;
            case 3:
                printf("Вы выбрали команду: Распечатать стек\n");
               	stackPrint(&stack);
                break;
            case 4:
                printf("Отсортировать стек\n");
                stackInsertSort(&stack);
                break;
            case 5:
                printf("Вы выбрали команду: Закончить работу программы\n");
                stackRemove(&stack);
    		    return 0;
                break;
            default:
                printf("Ошибка: неправильный ввод. Попробуйте еще раз.\n");
                break;
        }
	}

    return 0;
}