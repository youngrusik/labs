#include "list.h"

void printMenu() { // функция меню
    printf("\n");
    printf("0)Выход\n");
    printf("1)Распечатать список\n");
    printf("2)Добавить элемент в начало\n");
    printf("3)Добавить элемент в конец\n");
    printf("4)Добавить элемент по индексу\n");
    printf("5)Удалить элемент в начале\n");
    printf("6)Удалить элемент в конце\n");
    printf("7)Удалить элемент по индексу\n");
    printf("8)Узнать размер списка\n");
    printf("9)task\n");
    printf("\n");
    printf("Введите номер желаемого действия\n");
}

void cyclicShift(node** head) {
    if (*head == NULL || (*head)->next == *head) {
        return; // Список пустой или содержит только один элемент
    }

    node* last = (*head)->next; // Найдем последний элемент (первый после головы)
    while (last->next != *head) {
        last = last->next;
    }

    // Перемещаем последний элемент на первую позицию
    last->next = (*head)->next;
    (*head)->next = (*head)->next->next;
    last->next->next = *head;
    *head = last->next->next; // Обновляем указатель на голову
}

int main(int argc, const char *argv[]) {
    node* head = NULL; //изначально "голова" = нулю
    machineWord data;
    int choose = 1;
    int size = sizeList(head); //размер списка
    int index = 1;
    while(choose) {
        printMenu(); 
        scanf("%d", &choose);
        switch (choose) {
            case 0:
                printf("\nЗавершение работы...\n");
                destroy(head);
                return 0;
            case 1:
                printList(head);
                break;
            case 2:
                printf("Введите значение: ");
                readMachWord(&data);
                if(head == NULL) {
                    pushFront(&head, data);
                }
                pushFront(&head, data);
                break;
            case 3:
                printf("Введите значение: ");
                readMachWord(&data);
                if(head == NULL) {
                    pushBack(&head, data);
                }
                pushBack(&head, data);
                break;
            case 4:
                printf("Введите номер индекса: ");
                scanf("%d", &index);
                printf("Введите значение: ");
                readMachWord(&data);
                pushIndex(&head, data, index);
                break;
            case 5:
                deleteFront(&head);
                printf("Удаление завершено!\n");
                printList(head);
                break;
            case 6:
                deleteBack(&head);
                printf("Удаление завершено!\n");
                printList(head);
                break;
            case 7:
                printf("Введите номер индекса: ");
                scanf("%d", &index);
                deleteIndex(&head, index);
                if (size != sizeList(head)) {
                    printf("Удаление завершено!\n");
                }
                printList(head);
                break;
            case 8:
                size = sizeList(head);
                printf("Размер списка: %d\n", size);
                break;
            case 9:
                cyclicShift(&head);   
                printf("Done!\n");
                printList(head);              
                break;
            default:
                printf("Такого действия не существует\n");
                break;
        }
    }
    return 0;
}