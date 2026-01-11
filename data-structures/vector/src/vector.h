#pragma once

#include <stdlib.h>

#define VECTOR_SUCCESS 0
#define VECTOR_FAILURE -1

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
} vector;

int vector_create(vector* vec, size_t capacity, size_t element_size);
int vector_destroy(vector* vec);
int vector_push_back(vector* vec, void* element);
int vector_clear(vector* vec);