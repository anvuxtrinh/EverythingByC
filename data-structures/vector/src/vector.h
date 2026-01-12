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


/**
 *  @brief  Provides access to the data contained in the %vector.
 *  @param n The index of the element for which data should be
 *  accessed.
 *  @return  Read-only (constant) reference to data.
 *  @throw  std::out_of_range  If @a n is an invalid index.
 *
 *  This function provides for safer data access.  The parameter
 *  is first checked that it is in the range of the vector.  The
 *  function throws out_of_range if the check fails.
 */
void* vector_at(const vector* vec, size_t n);