#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "complex.h"

#include "matrix.h"
#include "streams.h"

char *readline_() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = (int) strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

void *read_row(char *str, size_t *n, RingInfo *ring_row) {
    *n = 0;
    char *token = strtok(str, " \t");
    void *row = NULL;
    while(token){
        int *buff = check_int(token);
        if (buff == NULL){
            free(row);
            free(buff);
            return NULL;
        }
        (*n)++;
        row = realloc(row, (*n) * sizeof(int));
        row[*n - 1] = *buff;
        free(buff);
        token = strtok(NULL, " \t");
    }
    if (!*n){
        free(row);
        return NULL;
    }
    return row;
}//

void *copy_row(void *dst, const void *src, size_t n, RingInfo ringInfo) {
    if (dst)
        free(dst);

    if (getSize(ringInfo) == sizeof(double complex)) {
        dst = calloc(n, sizeof(double complex));

        for (size_t i = 0; i < n; i++)
            ((double complex *) dst)[i] = ((const double complex *) src)[i];
    }
    else {
        dst = calloc(n, sizeof(int));

        for (size_t i = 0; i < n; i++)
            ((int *) dst)[i] = ((const int *) src)[i];
    }

    return dst;
}

Matrix *read_matrix_keyboard() {
    Matrix *ans = calloc(1, sizeof(Matrix));
    setRows(ans, 0);
    setColumns(ans, 0);
    setBody(ans, NULL);
    void *row = NULL;

    printf("Enter matrix. Use \"Ctrl + D\" to stop.\n");
    char *input = readline_();

    if (!input)
        return ans;

    while(input) {
        size_t n;
        RingInfo ring_row;
        row = read_row(input, &n, &ring_row);

        if (!getColumns(*ans)) {
            setColumns(ans, n);
        }

        if (!getRingInfo(*ans)) {
            setRingInfo(ans, &ring_row);
        }

        if (!row || n != getColumns(*ans) || getSize(ring_row) != getSize(*getRingInfo(*ans))) {

            if (n != getColumns(*ans))
                free(row);

            free(input);
            printf("Wrong input. Try again.\n");
            input = readline_();
            continue;
        }

        setRows(ans, getRows(*ans) + 1);

        if (getSize(*getRingInfo(*ans)) == sizeof(double complex))
            setBody(ans, realloc(getBody(*ans), getRows(*ans) * sizeof(double complex *)));
        else
            setBody(ans, realloc(getBody(*ans), getRows(*ans) * sizeof(int *)));

        row_insert(ans, row, getRows(*ans) - 1);

        free(row);
        free(input);

        input = readline_();
    }
    if (getSize(*getRingInfo(*ans)) == sizeof(double complex))
        printf("Entered %zu * %zu complex matrix.\n", getRows(*ans), getColumns(*ans));
    else
        printf("Entered %zu * %zu int matrix.\n", getRows(*ans), getColumns(*ans));

    return ans;
}

Matrix *read_matrix_binary_file() {
    Matrix *ans = calloc(1, sizeof(Matrix));
    setRows(ans, 0);
    setColumns(ans, 0);
    setBody(ans, NULL);
    FILE *file;
    size_t size;

    printf("Enter file path.\n");
    char* input = readline_();
    file = fopen(input, "rb");
    free(input);

    if (!file) {
        printf("Can't open file.\n");
        fclose(file);
        return ans;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    if (size < 3 * sizeof(size_t)) {
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    rewind(file);
    size_t mod;
    fread(&mod, sizeof(size_t), 1, file);
    if (mod == 0)
        setRingInfo(ans, NewRingComplex());
    else if (mod == 1)
        setRingInfo(ans, NewRingInt());
    else {
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    size_t rows, columns;

    fread(&rows, sizeof(size_t), 1, file);
    fread(&columns, sizeof(size_t), 1, file);

    setRows(ans, rows);
    setColumns(ans, columns);

    if ((mod == 0 && size != sizeof(double complex) * rows * columns + 3 * sizeof(size_t)) ||
    (mod == 1 && size != sizeof(int) * rows * columns + 3 * sizeof(size_t))) {
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    if (mod == 0) {
        double complex **ans_body = calloc(rows, sizeof(double complex *));
        for (size_t i = 0; i < rows; i++) {
            ans_body[i] = calloc(columns, sizeof(double complex));
            fread(ans_body[i], sizeof(double complex), columns, file);
        }
        setBody(ans, (void**) ans_body);
    } else {
        int **ans_body = calloc(rows, sizeof(int *));
        for(size_t i = 0; i < rows; i++){
            ans_body[i] = calloc(columns, sizeof(int));
            fread(ans_body[i], sizeof(int), columns, file);
        }
        setBody(ans, (void**) ans_body);
    }


    fclose(file);
    return ans;
}

void print_matrix(Matrix src) {
    for(size_t i = 0; i < src.rows; i++){
        if (getRingInfo(src)->size == sizeof(double complex))
            printf("%f + i%f", creal(((double complex **) getBody(src))[i][0]), cimag(((double complex **) getBody(src))[i][0]));
        else
            printf("%d", ((int **) getBody(src))[i][0]);
        for (size_t j = 1; j < src.columns; j++){
            if (getRingInfo(src)->size == sizeof(double complex))
                printf(" %f + i%f", creal(((double complex **) getBody(src))[i][j]), cimag(((double complex **) getBody(src))[i][j]));
            else
                printf(" %d", ((int **) getBody(src))[i][j]);
        }
        printf("\n");
    }
}

void print_matrix_binary_file(Matrix src) {
    FILE *file;

    printf("Enter file path.\n");
    char* input = readline_();
    file = fopen(input, "wb");

    if (!file){
        free(input);
        fclose(file);
        printf("Can't open file.\n");
        return;
    }
    size_t *dimensions = calloc(3, sizeof(size_t));
    dimensions[0] = getRingInfo(src)->size == sizeof(int) ? 1 : 0; //complex - 0, int - 1
    dimensions[1] = getRows(src);
    dimensions[2] = getColumns(src);
    fwrite(dimensions, sizeof(size_t), 2, file);
    free(dimensions);

    if (!getBody(src)){
        free(input);
        fclose(file);
        return;
    }

    for(size_t i = 0; i < getRows(src); i++)
        if (getRingInfo(src)->size == sizeof(double complex))
            fwrite(((double complex **) getBody(src))[i], getRingInfo(src)->size, getColumns(src), file);
        else
            fwrite(((int **) getBody(src))[i], getRingInfo(src)->size, getColumns(src), file);

    free(input);
    fclose(file);
}

int complex *check_complex(char *str) {
    if (!str)
        return NULL;
    char *token = strtok(str, " \t");
}

int *check_int(const char *str) {
    if (!str)
        return NULL;
    int len_max = (int) log10(pow(2,sizeof(int)*8 - 1) - 1) + 1;
    int sign = 1;
    int ans = 0;
    if (str[0] == '-')
        sign = -1;
    else if (str[0] == '0' && strlen(str) == 1)
        sign = 0;
    else if ((str[0] < 58) && (str[0] > 48))
        ans = ans * 10 + ((int) str[0] - 48);
    else
        return NULL;
    for (size_t i = 1; i < strlen(str); i++){
        if ((str[i] < 58) && (str[i] > 47))
            ans = ans * 10 + ((int) str[i] - 48);
        else
            return NULL;
    }

    if ((strlen(str) > len_max) || ans < 0)
        return NULL;
    ans *= sign;
    int *res = calloc(1, sizeof(int));
    memmove(res, &ans, 1 * sizeof(int));
    return res;
}

int gen_int() {
    return rand() % 2 ? -1 * rand() % (int) (pow(2, sizeof(int) * 8 - 1) - 1) : rand() % (int) (pow(2, sizeof(int) * 8 - 1) - 1);
}

int64_t check_natural(const char *s) {
    if (!s)
        return -1;
    size_t len = strlen(s);
    int64_t ans = 0;
    if (len >= 19) {
        printf("Too big amount\n");
        return -1;
    }
    if((*s == '0') && (len == 1))
        return 0;
    else if (*s == '0')
        return -1;
    while(*s){
        if(*s >= 48 && *s <= 57){
            ans = ans * 10 + ((int) *s - 48);
            s += 1;
        } else {
            return -1;
        }
    }

    return ans;
}

Matrix *gen_matrix(size_t n, size_t m) {
    Matrix *ans = calloc(1, sizeof(Matrix));

    setRows(ans, n);
    setColumns(ans, m);

    if(rand() % 2 == 0){
        setRingInfo(ans, NewRingComplex());
        double complex **ans_body = calloc(getRows(*ans), sizeof(double complex *));

        for (size_t i = 0; i < getRows(*ans); i++){
            ans_body[i] = calloc(getColumns(*ans), getRingInfo(*ans)->size);
            for (size_t j = 0; j < getColumns(*ans); j++){
                ans_body[i][j] = gen_int() + gen_int() * I;
            }
        }

        setBody(ans, (void **) ans_body);
    }
    else {
        setRingInfo(ans, NewRingInt());
        int **ans_body = calloc(getRows(*ans), sizeof(int *));

        for (size_t i = 0; i < getRows(*ans); i++){
            ans_body[i] = calloc(getColumns(*ans), getRingInfo(*ans)->size);
            for (size_t j = 0; j < getColumns(*ans); j++){
                ans_body[i][j] = gen_int();
            }
        }

        setBody(ans, (void **) ans_body);
    }

    return ans;
}
