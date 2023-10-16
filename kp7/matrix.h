#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SIZE 100

typedef struct {
    double* array;
    int size;
    int capacity;
} vector;

typedef struct {
    int rows, cols;
    vector* M;
    vector* A; 
} matrix;

vector* vector_create(int capacity);

void vector_print(vector* v);

void vector_push(vector* v, double value);

void free_vector(vector* v);

matrix* create_matrix(int rows, int cols);

matrix* read_matrix(FILE* fd);

void print_matrix(matrix* v);

double get(matrix* sp, int i, int j);

matrix* mult(matrix* a, matrix* b);

bool diagonal(matrix* sp);

void print_simple(matrix* sp);

void free_matrix(matrix* v);

#endif