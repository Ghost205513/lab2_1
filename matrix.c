#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "complex.h"
#include "matrix.h"



size_t getRows(Matrix matrix){
    return matrix.rows;
}

int setRows(Matrix *matrix, size_t rows){
    if (!matrix)
        return 1;
    matrix->rows = rows;
    return 0;
}

size_t getColumns(Matrix matrix){
    return matrix.columns;
}

int setColumns(Matrix *matrix, size_t columns){
    if (!matrix)
        return 1;
    matrix->columns = columns;
    return 0;
}

void ** getBody(Matrix matrix){
    return matrix.body;
}

int setBody(Matrix *matrix, void** body){
    if (!matrix)
        return 1;
    if (!body)
        return 1;
    matrix->body = body;
    return 0;
}

RingInfo *getRingInfo(Matrix matrix){
    return matrix.ringinfo;
}

int setRingInfo(Matrix *matrix, RingInfo *ringinfo){
   if (!matrix)
       return 1;
   matrix->ringinfo = ringinfo;
    return 0;
}

void free_all(Matrix *src){
    if (!src->body){
        free(src);
        return;
    }
    if (getRingInfo(*src)->size == sizeof(double complex)) {
        for (size_t i = 0; i < src->rows; i++)
            free(((double complex **) src->body)[i]);
        free((double complex *) src->body);
    }
    else {
        for (size_t i = 0; i < src->rows; i++)
            free(((int **) src->body)[i]);
        free((int *) src->body);
    }

    free(src->ringinfo);
    src->body = NULL;
    src->rows = 0;
    src->columns = 0;
    free(src);
}

void free_matrix(Matrix *src){
    if (!src->body){
        free(src);
        return;
    }
    if (getRingInfo(*src)->size == sizeof(double complex)) {
        for (size_t i = 0; i < src->rows; i++)
            free(((double complex **) src->body)[i]);
    }
    else {
        for (size_t i = 0; i < src->rows; i++)
            free(((int **) src->body)[i]);
    }

    free(src->body);
    src->body = NULL;
    src->rows = 0;
    src->columns = 0;
    free(src);
}

void free_body(Matrix *src){
    if (!src->body)
        return;

    for (size_t i = 0; i < src->rows; i++)
        free(src->body[i]);

    free(src->body);
    src->body = NULL;
    src->rows = 0;
    src->columns = 0;
}

Matrix *copy_matrix(Matrix *dst, Matrix *src){
    if (dst->body)
        free_body(dst);

    if (!dst)
        dst = calloc(1, sizeof(Matrix));

    dst->ringinfo = src->ringinfo;
    dst->rows = src->rows;
    dst->columns = src->columns;

    if (src->ringinfo->size == sizeof(double complex)){
        if (src->body)
            dst->body = calloc(src->rows, sizeof(double complex *));
    }
    else {
        if (src->body)
            dst->body = calloc(src->rows, sizeof(int *));
    }

    for (size_t i = 0; i < src->rows; i++){
        dst->body[i] = calloc(src->columns, src->ringinfo->size);
        memmove(dst->body[i], src->body[i], src->columns * src->ringinfo->size);
    }

    return dst;
}

void row_insert(Matrix *dst, int *row, size_t index){
    dst->body = realloc(dst->body, (dst->rows + 1) * sizeof(int *));
    dst->body[dst->rows] = calloc(dst->columns, sizeof(int));

    for (size_t i = dst->rows; i > index; i--)
        memmove(dst->body[i] , dst->body[i - 1], dst->columns * sizeof(int));

    memmove(dst->body[index], row, dst->columns * sizeof(int));
    dst->rows++;
}//

void column_insert(Matrix *src, const int *column, size_t index){
    for (size_t i = 0; i < src->rows; i++){
        src->body[i] = realloc(src->body[i], (src->columns + 1) * sizeof(int));
        memmove(&src->body[i][index + 1], &src->body[i][index], (src->columns - index) * sizeof(int));
        src->body[i][index] = column[i];
    }

    src->columns++;
}//

void row_delete(Matrix *src, size_t index){
    if (src->rows == 1){
        free_body(src);
        return;
    }
    for (size_t i = index; i < src->rows - 1; i++)
        memmove(src->body[i], src->body[i + 1], src->columns * sizeof(int));

    free(src->body[src->rows - 1]);
    src->body = realloc(src->body, (src->rows - 1) * sizeof(int *));
    src->rows--;
}//

void column_delete(Matrix *src, size_t index){
    if (src->columns == 1){
        free_body(src);
        return;
    }
    for (size_t i = 0; i < src->rows; i++){
        memmove(&src->body[i][index], &src->body[i][index + 1], (src->columns - index - 1) * sizeof(int));
        src->body[i] = realloc(src->body[i], (src->columns - 1) * sizeof(int));
    }

    src->columns--;
}//

/*int inversions(const size_t *permutation, size_t n){
    size_t *sorted;
    int ans = 1;
    sorted = calloc(n, sizeof(size_t));
    for (size_t i = 0; i < n; i++)
        sorted[i] = i;
    for (size_t i = 0; i < n; i++){
        if (sorted[permutation[i]] % 2 == 1)
            ans *= -1;
        for (size_t j = permutation[i] + 1; j < n; j++)
            if (sorted[j])
                sorted[j]--;
    }
    free(sorted);
    return ans;
}*/

/*int64_t determinant(Matrix *src, size_t index, bool *used, size_t *generate){
    int64_t ans = 0;
    if (index == src->columns){
        ans = inversions(generate, index);
        for (size_t i = 0; i < index; i++)
            ans *= src->body[i][generate[i]];
        return ans;
    }
    for (size_t i = 0; i < src->columns; i++){
        if (used[i])
            continue;
        generate[index] = i;
        used[i] = true;
        ans += determinant(src, index + 1, used, generate);
        used[i] = false;
    }
    return ans;
}*/

/*int64_t det(Matrix *src){
    bool *used = calloc(src->columns, sizeof(bool));
    size_t *generate = calloc(src->columns, sizeof(size_t));
    int64_t ans = determinant(src, 0, used, generate);

    free(used);
    free(generate);

    return ans;
}*/

Matrix *sum(Matrix *a, Matrix *b){
    Matrix *ans = calloc(1, sizeof(Matrix));

    if (getRingInfo(*a)->size == sizeof(double complex)) {
        double complex **body_sum = calloc(getRows(*b), sizeof(double complex *));
        for(size_t i = 0; i < getRows(*b); i++){
            body_sum[i] = calloc(getColumns(*b), sizeof(double complex));
            for(size_t j = 0; j < getColumns(*b); j++){
                body_sum[i][j] = *((double complex *) getRingInfo(*a)->sum(&getBody(*b)[i][j], &getBody(*a)[i][j]));
            }
        }
        setBody(ans, (void **) body_sum);
    }else {
        int **body_sum = calloc(getRows(*b), sizeof(int *));
        for(size_t i = 0; i < getRows(*b); i++){
            body_sum[i] = calloc(getColumns(*b), sizeof(int));
            for(size_t j = 0; j < getColumns(*b); j++){
                body_sum[i][j] = *((int *) getRingInfo(*a)->sum(&getBody(*b)[i][j], &getBody(*a)[i][j]));
            }
        }
        setBody(ans, (void **) body_sum);
    }

    setRingInfo(ans, getRingInfo(*a));
    setRows(ans, getRows(*a));
    setColumns(ans, getColumns(*a));

    return ans;
}

Matrix *multiply(Matrix *a, Matrix *b){
    Matrix *ans = calloc(1, sizeof(Matrix));

    if (getRingInfo(*a)->size == sizeof(double complex)) {
        double complex **body_multiply = calloc(getRows(*a), sizeof(double complex *));
        for(size_t i = 0; i < getRows(*a); i++){
            body_multiply[i] = calloc(getColumns(*b), sizeof(double complex));
            for(size_t j = 0; j < getColumns(*b); j++){
                body_multiply[i][j] = *((double complex *) getRingInfo(*a)->zero);
                for(size_t k = 0; k < getColumns(*a); k++)
                    body_multiply[i][j] += *((double complex *) getRingInfo(*a)->multiply(&getBody(*a)[i][j + k], &getBody(*b)[i + k][j]));
            }
        }
        setBody(ans, (void **) body_multiply);
    }else {
        int **body_multiply = calloc(getRows(*a), sizeof(int *));
        for(size_t i = 0; i < getRows(*a); i++){
            body_multiply[i] = calloc(getColumns(*b), sizeof(int));
            for(size_t j = 0; j < getColumns(*b); j++){
                body_multiply[i][j] = *((int *) getRingInfo(*a)->zero);
                for(size_t k = 0; k < getColumns(*a); k++)
                    body_multiply[i][j] += *((int *) getRingInfo(*a)->multiply(&getBody(*a)[i][j + k], &getBody(*b)[i + k][j]));
            }
        }
        setBody(ans, (void **) body_multiply);
    }

    setRingInfo(ans, getRingInfo(*a));
    setRows(ans, getRows(*a));
    setColumns(ans, getColumns(*b));

    return ans;
}

Matrix *transpose(Matrix *a){
    Matrix *ans = calloc(1, sizeof(Matrix));

    if (getRingInfo(*a)->size == sizeof(double complex)) {
        double complex **body_transpose = calloc(getColumns(*a), sizeof(double complex *));
        for(size_t i = 0; i < getColumns(*a); i++)
            body_transpose[i] = calloc(getRows(*a), sizeof(double complex));
        for(size_t i = 0; i < getRows(*a); i++){
            for(size_t j = 0; j < getColumns(*a); j++)
                body_transpose[j][i] = ((double complex **) getBody(*a))[i][j];
        }
        setBody(ans, (void **) body_transpose);
    }else {
        int **body_transpose = calloc(getColumns(*a), sizeof(int *));
        for(size_t i = 0; i < getColumns(*a); i++)
            body_transpose[i] = calloc(getRows(*a), sizeof(int));
        for(size_t i = 0; i < getRows(*a); i++){
            for(size_t j = 0; j < getColumns(*a); j++){
                body_transpose[j][i] = ((int **) getBody(*a))[i][j];
            }
        }
        setBody(ans, (void **) body_transpose);
    }

    setRingInfo(ans, getRingInfo(*a));
    setRows(ans, getColumns(*a));
    setColumns(ans, getRows(*a));

    return ans;
}
