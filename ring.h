#include "stdint.h"

typedef struct RingInfo{
    size_t size;
    void *(*sum)(void*, void*);
    void *(*minus)(void*, void*);
    void *zero;
    void *(*multiply)(void*, void*);
    void *(*divide)(void*, void*);
    void *one;
}RingInfo;

size_t getSize(RingInfo ringInfo);

RingInfo *Create(size_t size, void *(*sum)(void*, void*), void *(*minus)(void*, void*), void *zero,
                 void *(*multiply)(void*, void*), void *(*divide)(void*, void*), void *one);

RingInfo *NewRingInt();

RingInfo *NewRingComplex();
