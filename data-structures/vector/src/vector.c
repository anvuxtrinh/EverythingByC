#include <assert.h>
#include <string.h>
#include "vector.h"

int vector_create(vector* vec, size_t capacity, size_t element_size) {
    assert(vec != NULL);

    if(vec == NULL){
        return VECTOR_FAILURE;
    }

    vec->data = malloc(capacity * element_size);
    if (!vec->data) {
        free(vec);
        return VECTOR_FAILURE;
    }

    memset(vec->data, 0, capacity * element_size);
    vec->size = capacity;
    vec->capacity = capacity;
    vec->element_size = element_size;
    return VECTOR_SUCCESS;
}

int vector_push_back(vector* vec, void* element){
    assert(vec != NULL && element != NULL);

    if(vec == NULL || element == NULL){
        return VECTOR_FAILURE;
    }

    //If the size exceeds capacity, reallocate memory
    if(vec->size >= vec->capacity){
        size_t new_capacity = vec->capacity * 2;
        void* new_data = realloc(vec->data, new_capacity*vec->element_size);
        if(!new_data){
            return VECTOR_FAILURE;
        }
        memset(new_data, 0, new_capacity * vec->element_size);
        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    //Copy the element to the end of the vector
    memcpy(vec->data + vec->element_size * vec->size, element, vec->element_size);
    ++vec->size;
    return VECTOR_SUCCESS;
}

int vector_clear(vector* vec){
    assert(vec != NULL);

    if(vec == NULL){
        return VECTOR_FAILURE;
    }

    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    return VECTOR_SUCCESS;
}

int vector_destroy(vector* vec) {
    assert(vec != NULL);

    if(vec == NULL){
        return VECTOR_FAILURE;
    }

    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    return VECTOR_SUCCESS;
}