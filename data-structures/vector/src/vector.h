#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define VECTOR_SUCCESS 0
#define VECTOR_FAILURE -1

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
} vector;

typedef struct {
    size_t element_size;
    void *ptr;
} iterator;

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

/**
 *  @brief  Assigns a given value to a %vector.
 *  @param  n  Number of elements to be assigned.
 *  @param  val  Value to be assigned.
 *
 *  This function fills a %vector with @a n copies of the given
 *  value.  Note that the assignment completely changes the
 *  %vector and that the resulting %vector's size is the same as
 *  the number of elements assigned.
 */
int vector_assign(vector* vec, size_t n, void* val);
void* vector_front(vector* vec);
void* vector_back(vector* vec);
void* vector_data(vector* vec);
bool vector_empty(vector* vec);
size_t vector_size(vector* vec);
int vector_resize(vector* vec, size_t new_size, void* val);
int vector_swap(vector* vec1, vector* vec2);

iterator vector_iterator(vector* vec, int idx);
iterator vector_begin(vector* vec);
iterator vector_end(vector* vec);
iterator vector_emplace(vector* vec, const iterator it, void* val);
iterator vector_emplace_back(vector* vec, void* val);
