#include "matrix.h"

// Функция для создания вектора
vector* vector_create(int capacity) {
    vector* v = (vector*)malloc(sizeof(vector));
    v->size = 0;
    v->capacity = capacity;
    v->array = calloc(capacity, sizeof(double));
    return v;
}

// Функция для вывода значений вектора
void vector_print(vector* v) {
    if (v == NULL) {
        return;
    }
    for (int i = 0; i < v->size; i++) {
        printf("(%.1lf) ", v->array[i]);
    }
    printf("\n");
}

// Функция для добавления значения в вектор
void vector_push(vector* v, double value) {
    if (v->size >= v->capacity) {
        v->capacity++;
        v->array = (double*)realloc(v->array, v->capacity * sizeof(double));
    }
    v->array[v->size++] = value;
}

// Функция для освобождения выделенной памяти для вектора
void free_vector(vector* v) {
    free(v->array);
    v->array = NULL;
    free(v);
}

// Функция для создания матрицы
matrix* create_matrix(int rows, int cols) {
    matrix* sp = (matrix*)malloc(sizeof(matrix));
    sp->rows = rows;
    sp->cols = cols;
    sp->A = vector_create(rows);
    sp->M = vector_create(cols * rows);
    return sp;
}

// Функция для чтения матрицы из файла и преобразования ее в разреженное представление
matrix* read_matrix(FILE* fd) {
    int rows, cols;
    if (fscanf(fd, "%d %d", &rows, &cols) != 2) {
        return NULL; // Не удалось прочитать размер матрицы, поэтому предполагаем, что достигнут конец файла.
    }
    double m[rows][cols]; // Создаем двумерный массив для хранения значений матрицы.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(fd, "%lf", &m[i][j]); // Считываем значения элементов матрицы из файла.
        }
    }
    matrix* sp = create_matrix(rows, cols); // Создаем разреженное представление матрицы.
    for (int i = 0; i < rows; i++) {
        int row_index = sp->A->size + 1; // Индекс начала строки в разреженном представлении.
        bool has_values = false; // Флаг для отслеживания наличия ненулевых значений в строке.
        for (int j = 0; j < cols; j++) {
            if (m[i][j] == 0.0) {
                continue; // Если элемент матрицы равен 0, переходим к следующему столбцу.
            }
            has_values = true;
            vector_push(sp->A, j + 1); // Добавляем индекс столбца и значение в разреженное представление.
            vector_push(sp->A, m[i][j]);
        }
        if (has_values) {
            vector_push(sp->M, row_index); // Если в строке есть ненулевые значения, добавляем индекс начала строки в M.
        } else {
            vector_push(sp->M, 0); // В противном случае добавляем 0 для обозначения пустой строки.
        }
    }
    return sp; // Возвращаем разреженное представление матрицы.
}


// Функция для вывода разреженного представления матрицы
void print_matrix(matrix* sp) {
    printf("M: ");
    vector_print(sp->M);
    printf("A: ");
    vector_print(sp->A);
}

// Функция для вывода разреженного представления матрицы как обычной матрицы
void print_simple(matrix* sp) {
    for (int i = 0; i < sp->rows; i++) {
        double row[sp->cols];
        for (int r = 0; r < sp->cols; r++) {
            row[r] = 0.0;
        }
        if (sp->M->array[i] != 0.0) {
            int left = sp->M->array[i];
            int right = sp->A->size;
            if (i + 1 < sp->rows) {
                right = sp->M->array[i + 1];
            }
            for (int j = left - 1; j < right - 1; j += 2) {
                int col_idx = sp->A->array[j] - 1;
                int value = sp->A->array[j + 1];
                row[col_idx] = value;
            }
        }
        for (int r = 0; r < sp->cols; r++) {
            printf("|%.1lf| ", row[r]);
        }
        printf("\n");
    }
}

// Функция для получения значения матрицы в определенной строке и столбце
double get(matrix* sp, int row, int col) {
    if (sp->M->array[row] == 0) {
        return 0.0; // Если индекс строки в M равен 0, значит, нет ненулевых элементов в этой строке.
    }
    int left = sp->M->array[row];
    int right = sp->A->size;
    if (row + 1 < sp->rows) {
        right = sp->M->array[row + 1];
    }
    for (int j = left - 1; j < right - 1; j += 2) {
        int col_idx = sp->A->array[j] - 1;
        int value = sp->A->array[j + 1];
        if (col_idx == col) {
            return value; // Возвращаем значение элемента матрицы в указанной строке и столбце.
        }
    }
    return 0.0; // Если элемент не найден, возвращаем 0.0.
}

// Функция для умножения двух матриц
matrix* mult(matrix* a, matrix* b) {
    if (a->cols != b->rows) {
        return NULL; // Проверяем, можно ли умножить матрицы; если нет, возвращаем NULL.
    }
    matrix* res = create_matrix(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        int row_index = res->A->size + 1;
        bool has_values = false;
        for (int j = 0; j < b->cols; j++) {
            double rslt = 0.0;
            for (int k = 0; k < b->rows; k++) {
                rslt += get(a, i, k) * get(b, k, j); // Умножаем соответствующие элементы и суммируем результаты.
            }
            if (rslt == 0.0) {
                continue; // Если результат равен 0, переходим к следующему столбцу.
            }
            has_values = true;
            vector_push(res->A, j + 1); // Добавляем индекс столбца и результат в разреженное представление матрицы res.
            vector_push(res->A, rslt);
        }
        if (has_values) {
            vector_push(res->M, row_index); // Если в строке есть ненулевые значения, добавляем индекс начала строки в M.
        } else {
            vector_push(res->M, 0); // В противном случае добавляем 0 для обозначения пустой строки.
        }
    }
    return res; // Возвращаем результат умножения матриц a и b.
}

// Функция для проверки, является ли матрица диагональной
bool diagonal(matrix* sp) {
    if (sp->rows != sp->cols) {
        return false; // Диагональной матрицей должна быть квадратная матрица.
    }

    for (int i = 0; i < sp->rows; i++) {
        for (int j = 0; j < sp->cols; j++) {
            if (i != j && get(sp, i, j) != 0.0) {
                return false; // Недиагональный элемент не равен нулю.
            }
        }
    }
    return true; // Все недиагональные элементы равны нулю.
}

// Функция для освобождения памяти, выделенной для матрицы
void free_matrix(matrix* m) {
    free_vector(m->M);
    free_vector(m->A);
    free(m);
}
