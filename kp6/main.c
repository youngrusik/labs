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

bool ent_eq_points(Entrant e1, Entrant e2) {
    return (e1.math == e2.math) && (e1.russian == e2.russian) && (e1.informatics == e2.informatics);
}

bool ent_eq(Entrant e1, Entrant e2) {
    return (e1.essay == e2.essay) && (e1.gender == e2.gender) && (e1.have_medal == e2.have_medal)
        && (e1.informatics == e2.informatics) && (strcmp(e1.initials, e2.initials) == 0)
        && (e1.math == e2.math) && (e1.russian == e2.russian) && (e1.school_num == e2.school_num)
        && (strcmp(e1.surname, e2.surname) == 0);
}

bool check_points(Entrant e, int p) {
    return ((e.math + e.russian + e.informatics) >= p);
}

// if success returns true
bool add_record(FILE* db, Entrant e) {
    // printf("DEBUG 1\n");
    fseek(db, 0, SEEK_END);
    // printf("DEBUG 2\n");
    if (fwrite(&e, sizeof(Entrant), 1, db) != 1) {
        return false;
    }
    return true;
}

// return find or not
bool find_record(FILE* db, Entrant e) {
    fseek(db, sizeof(int), SEEK_SET);
    Entrant cur;
    bool is_found = false;
    int fread_exit_code = fread(&cur, sizeof(Entrant), 1, db);
    while (fread_exit_code == 1) {
        if ((strcmp(cur.surname, e.surname) == 0) && (strcmp(cur.initials, e.initials) == 0)) {
            is_found = true;
            break;
        }
        fread_exit_code = fread(&cur, sizeof(Entrant), 1, db);
    }
    if (fread_exit_code == -1) {
        perror("fread error!");
        exit(1);
    }
    return is_found;
}

void interface(FILE* db) {
    while (true) {
        printf("---------------------\n");
        printf("1. Find entrant\n");
        printf("2. Add new entrant\n");
        printf("3. Exit\n");
        printf("Choose func: ");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            Entrant new;
            printf("Enter surname: ");
            scanf("%s", new.surname);
            printf("Enter initials: ");
            scanf("%s", new.initials);
            bool res = find_record(db, new);
            if (res) {
                printf("Founded!\n");
            } else {
                printf("Not founded!\n");
            }
        } else if (choice == 2) {
            Entrant new;
            printf("Enter surname: ");
            scanf("%s", new.surname);
            printf("Enter initials: ");
            scanf("%s", new.initials);
            printf("Enter school number: ");
            scanf("%d", &new.school_num);
            printf("Enter math points: ");
            scanf("%d", &new.math);
            printf("Enter russian points: ");
            scanf("%d", &new.russian);
            printf("Enter informatics points: ");
            scanf("%d", &new.informatics);
            printf("Enter result of essay (1/0): ");
            scanf("%d", &new.essay);
            printf("Do entrant have a medal? (1/0): ");
            scanf("%d", &new.have_medal);
            printf("Enter gender: ");
            scanf(" %c", &new.gender);
            if (!add_record(db, new)) {
                perror("add error!");
                exit(1);
            }
        } else if (choice == 3) {
            return;
        }
    }
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
    FILE* input = fopen(argv[1], "ab+");
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
        if (ent_eq_points(ens[i], ens[i + 1]) && !check_points(ens[i], p) && !check_points(ens[i + 1], p) && ens[i].gender == 'F' && ens[i + 1].gender == 'F') {
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

    interface(input);

    fclose(input);

    return 0;
}
