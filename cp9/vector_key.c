#include "vector_key.h"

void createVector(vector* v, size_t size) {
    v->size = size;
    v->data = (value*)malloc(size * sizeof(value));
    for (int i = 0; i < size; i++) {
        v->data[i].key = 0;
        strcpy(v->data[i].string, "");
    }
}

bool empty(vector* v) {
    return v->size == 0;
}

int sizeVector(vector* v) {
    return v->size;
}

value loadVector(vector* v, long i) {
    value emptyValue;
    emptyValue.key = 0;
    strcpy(emptyValue.string, "");

    if (i >= 0 && i < v->size) {
        return v->data[i];
    } else {
        return emptyValue;
    }
}

void writeVector(vector* v, int i, value val) {
    if (i >= 0 && i < v->size) {
        v->data[i] = val;
    }
}

void resizeVector(vector* v, size_t size) {
    value* new_values = (value*)realloc(v->data, size * sizeof(value));
    v->data = new_values;
    v->size = size;

    if (v->size > size) {
        for (int i = size; i < v->size; i++) {
            v->data[i].key = 0;
            strcpy(v->data[i].string, "");
        }
    }
}

void destroyVector(vector* v) {
    v->size = 0;
    free(v->data);
    v->data = NULL;
}

void printTable(vector* v) {
    int size = sizeVector(v);
    printf("|============================================|\n");
    printf("|  Ключ        | Значение                          |\n");
    printf("|============================================|\n");
    for (int i = 0; i < size; i++) {
        value tmp = loadVector(v, i);
        printf("| %12ld | %-41s \n", tmp.key, tmp.string);
    }
    printf("|============================================|\n");
}

void swap(value* x, value* y) {
    value temp = *x;
    *x = *y;
    *y = temp;
}

value binarySearch(vector* table, long key) {
    long left = 0;
    long right = sizeVector(table) - 1;
    long middle;
    value def = { 0, "" }; //если ничено не найдем, то вернем пустой объект
    while (left <= right) {
        middle = (left + right) / 2;

        if (key < table->data[middle].key) {
            right = middle - 1;
        } else if (key > table->data[middle].key) {
            left = middle + 1;
        } else {
            return table->data[middle];
        }
    }

    return def;
}

value reverseBinarySearch(vector* table, long key) {
    long left = sizeVector(table) - 1;
    long right = 0;
    value def = { 0, "" }; // Если ничего не найдено, вернем пустой объект

    while (left >= right) {
        long middle = (left + right) / 2;

        if (key < table->data[middle].key) {
            right = middle + 1;
        } else if (key > table->data[middle].key) {
            left = middle - 1;
        } else {
            return table->data[middle];
        }
    }

    return def;
}

bool checkSortTable(vector* table) { // функция проверки на отсортированность таблицы
    int size = sizeVector(table);
    for (int i = 0; i < size - 1; i++) {
        value a = loadVector(table, i);
        value b = loadVector(table, i + 1);

        if (a.key > b.key) { //если нарушен порядок
            return false;
        }
    }

    return true;
}

bool checkSortTableDescending(vector* table) {// функция проверки на убывание(обратные порядок)
    int size = sizeVector(table);
    for (int i = 0; i < size - 1; i++) {
        value a = loadVector(table, i);
        value b = loadVector(table, i + 1);

        if (a.key < b.key) { // если нарушен порядок
            return false;
        }
    }

    return true;
}

void sortInsertBinary(vector* table) {
    int size = sizeVector(table);

    for (int i = 1; i < size; i++) {
        value current = loadVector(table, i);
        int j = i - 1;

        while (j >= 0 && loadVector(table, j).key > current.key) {
            value temp = loadVector(table, j);
            writeVector(table, j + 1, temp);
            j--;
        }

        writeVector(table, j + 1, current);
    }
}