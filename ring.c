#include "ring.h"
#include "stdlib.h"
#include "complex.h"

size_t getSize(RingInfo ringInfo){
    return ringInfo.size;
}

RingInfo *Create(size_t size, void *(*sum)(void*, void*), void *(*minus)(void*, void*), void *zero,
                 void *(*multiply)(void*, void*), void *(*divide)(void*, void*), void *one){
    RingInfo *result = malloc(sizeof(RingInfo));

    result->size = size;
    result->sum = sum;
    result->minus = minus;
    result->zero = zero;
    result->multiply = multiply;
    result->divide = divide;
    result->one = one;

    return result;
}

size_t sizeInt = sizeof(int);

void *sumInt(void *a, void *b){
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    int *result = malloc(sizeof(int));
    *result = *int_a + *int_b;
    return (void *)result;
}

void *minusInt(void *a, void *b){
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    int *result = malloc(sizeof(int));
    *result = *int_a - *int_b;
    return (void *)result;
}

int zeroInt = 0;

void *multiplyInt(void *a, void *b){
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    int *result = malloc(sizeof(int));
    *result = *int_a * *int_b;
    return (void *)result;
}

void *divideInt(void *a, void *b){
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    int *result = malloc(sizeof(int));
    *result = *int_a / *int_b;
    return (void *)result;
}

int oneInt = 1;

size_t sizeComplex = sizeof(double complex);

void *sumComplex(void *a, void *b){
    double complex *complex_a = (double complex *)a;
    double complex *complex_b = (double complex *)b;
    double complex *result = malloc(sizeof(double complex));
    *result = *complex_a + *complex_b;
    return (void *)result;
}

void *minusComplex(void *a, void *b){
    double complex *complex_a = (double complex *)a;
    double complex *complex_b = (double complex *)b;
    double complex *result = malloc(sizeof(double complex));
    *result = *complex_a - *complex_b;
    return (void *)result;
}

double complex zeroComplex = 0.0 + 0.0 * I;

void *multiplyComplex(void *a, void *b){
    double complex *complex_a = (double complex *)a;
    double complex *complex_b = (double complex *)b;
    double complex *result = malloc(sizeof(double complex));
    *result = *complex_a * *complex_b;
    return (void *)result;
}

void *divideComplex(void *a, void *b){
    double complex *complex_a = (double complex *)a;
    double complex *complex_b = (double complex *)b;
    double complex *result = malloc(sizeof(double complex));
    *result = *complex_a / *complex_b;
    return (void *)result;
}

double complex oneComplex = 1.0 + 0.0 * I;

RingInfo *NewRingInt(){
    RingInfo *result = Create(sizeInt, sumInt, minusInt, &zeroInt, multiplyInt, divideInt, &oneInt);
    return result;
}

RingInfo *NewRingComplex(){
    RingInfo *result = Create(sizeComplex, sumComplex, minusComplex, &zeroComplex, multiplyComplex, divideComplex, &oneComplex);
    return result;
}
