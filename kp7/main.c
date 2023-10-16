#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_SIZE 50

int main(int argc, char **argv) {
    if (argc != 2 || argv[1] == NULL) {
        printf("Использование: lab <файл>\n");
        exit(1);
    }

    FILE* fd = fopen(argv[1], "r");
    if (fd == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    }
    matrix* list[LIST_SIZE];
    matrix* sp;

    int n = 0;

    while (true) {
        sp = read_matrix(fd);
        if (sp == NULL) {
            break;
        }
        list[n++] = sp;
        printf("{ #%d }\n", n);
        printf("Разреженная матрица:\n");
        print_matrix(sp);
        printf("Обычная матрица:\n");
        print_simple(sp);
        printf("\n\n");

    }
    int a, b;
    while (1) {
        printf("Введите номера двух матриц, которые нужно умножить:\n");
        printf("(Чтобы выйти, введите '0')\n");
        scanf("%d", &a);
        if (a == 0) {
            break;
        }
        scanf("%d", &b);
        if (a < 1 || a > n || b < 1 || b > n) {
            printf("Неверно выбраны матрицы!\n\n");
            continue;
        }
        matrix* res = mult(list[a-1], list[b-1]);
        if (res == NULL) {
            printf("Нельзя перемножить эти матрицы!\n\n");
        } else {
            printf("\nРезультат умножения матриц %d и %d:\n", a, b);
            print_matrix(res);
            print_simple(res);
            if (diagonal(res)) {
                printf("Матрица является диагональной\n");
            } else {
                printf("Матрица не является диагональной\n");
            }
            printf("==---------------==\n\n");
        }
    }
    

    for (int i = 0; i < n; i++) {
        free_matrix(list[i]);
    }

    return 0;
}