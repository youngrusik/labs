#include "entrant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool cmp(Entrant e1, Entrant e2) {
    if (e1.math != e2.math) {
        return e1.math > e2.math;
    }
    if (e1.russian != e2.russian) {
        return e1.russian > e2.russian;
    }
    return e1.informatics > e2.informatics;
}

void swap(Entrant* a, Entrant* b)
{
    Entrant t = *a;
    *a = *b;
    *b = t;
}

int partition(Entrant arr[], int start, int end)
{
    Entrant pivot = arr[start];
 
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (!cmp(arr[i], pivot)) {
            count++;
        }
    }
 
    int pivotIndex = start + count;
    swap(&arr[pivotIndex], &arr[start]);
 
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {

        while (!cmp(arr[i], pivot)) {
            i++;
        }
 
        while (cmp(arr[j], pivot)) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            swap(&arr[i++], &arr[j--]);
        }
    }
 
    return pivotIndex;
}
 
void quickSort(Entrant arr[], int start, int end)
{
 
    if (start >= end) {
        return;
    }
 
    int p = partition(arr, start, end);
 
    quickSort(arr, start, p - 1);
 
    quickSort(arr, p + 1, end);
}

bool ent_eq(Entrant e1, Entrant e2) {
    return (e1.math == e2.math) && (e1.russian == e2.russian) && (e1.informatics == e2.informatics);
}

bool check_points(Entrant e, int p) {
    return ((e.math + e.russian + e.informatics) >= p);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        perror("Usage: ./kp06 FILENAME -p POINTS");
        exit(1);
    }
    if (strcmp(argv[2], "-p") != 0) {
        perror("Usage: ./kp06 FILENAME -p POINTS");
        exit(1);
    }
    FILE* input = fopen(argv[1], "rb");
    int p = atoi(argv[3]);
    if (input == NULL){
        perror("Can't open input file!");
        exit(1);
    }
    int n;
    if (fread(&n, sizeof(int), 1, input) != 1) {
        perror("Read number of elems error!");
        exit(1);
    }
    fseek(input, sizeof(int), SEEK_SET);

    Entrant ens[n];
    Entrant e;
    for (int i = 0; i < n; ++i) {
        if (fread(&e, sizeof(Entrant), 1, input) != 1) {
            perror("Read elem error!");
            exit(1);
        }
        ens[i] = e;
    }

    quickSort(ens, 0, n - 1);

    bool is_prev_eq = false;

    for (int i = 0; i < n - 1; ++i) {
        if (ent_eq(ens[i], ens[i + 1]) && !check_points(ens[i], p) && !check_points(ens[i + 1], p)) {
            printf("%s %s\n", ens[i].surname, ens[i].initials);
            is_prev_eq = true;
        } else if (is_prev_eq){
            printf("%s %s\n", ens[i].surname, ens[i].initials);
            is_prev_eq = false;
        }
    }
    if (is_prev_eq) {
        printf("%s %s\n", ens[n - 1].surname, ens[n - 1].initials);
    }

    fclose(input);

    return 0;
}
