#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STRSIZE 100

typedef struct _value {
    long key; //ключ
    char string[STRSIZE]; //строка стихотворения
} value;

typedef struct _vector{ //структура вектора
    value* data; //значение
    size_t size; //размер 
} vector;

void createVector(vector* v, size_t size);
bool empty(vector* v);
int sizeVector(vector* v);
value loadVector(vector* v, long i);
void writeVector(vector* v, int i, value val);
void resizeVector(vector* v, size_t size);
void destroyVector(vector* v);
void printTable(vector* v);

void swap(value* x, value* y);
value binarySearch(vector* table, long key);
value reverseBinarySearch(vector* table, long key);
bool checkSortTable(vector* table);
bool checkSortTableDescending(vector* table);

void sortInsertBinary(vector* table);
#endif //__VECTOR_H__