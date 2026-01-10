#pragma once

#include <stdlib.h>

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
} vector;


vector* vector_create(size_t element_size);
void vector_destroy(vector* vec);
int vector_push_back(vector* vec, void* element, size_t element_size);
int vector_clear();