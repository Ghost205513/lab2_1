#include "ring.h"

typedef struct Matrix{
    RingInfo *ringinfo;
    size_t rows;
    size_t columns;
    void **body;
}Matrix;

size_t getRows(Matrix matrix);

int setRows(Matrix *matrix, size_t rows);

size_t getColumns(Matrix matrix);

int setColumns(Matrix *matrix, size_t columns);

void ** getBody(Matrix matrix);

int setBody(Matrix *matrix, void** body);

RingInfo *getRingInfo(Matrix matrix);

int setRingInfo(Matrix *matrix, RingInfo *ringinfo);

void free_all(Matrix *src);

void free_matrix(Matrix *src);

void free_body(Matrix *src);

Matrix *copy_Matrix(Matrix *dst, Matrix *src);

void row_insert(Matrix *dst, int *row, size_t index);

void column_insert(Matrix *src, const int *column, size_t index);

void row_delete(Matrix *src, size_t index);

void column_delete(Matrix *src, size_t index);

Matrix *sum(Matrix *a, Matrix *b);

Matrix *multiply(Matrix *a, Matrix *b);

Matrix *transpose(Matrix *a);