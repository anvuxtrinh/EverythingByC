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

void* vector_at(const vector* vec, size_t pos){
    assert(vec != NULL && vec->size > pos);

    if(vec == NULL || vec->size <= pos){
        return NULL;
    }

    return vec->data + pos*vec->element_size;
}

void vector_assign(vector* vec, size_t n, void* val) {
    assert(vec != NULL && val != NULL);

    if(vec == NULL || val == NULL)
        return;
    
    // In case size of assign requirement is greater current capacity
    if(vec->capacity < n){
        void *new_data = realloc(vec->data, n*vec->element_size);
        if(new_data == NULL){
            return;
        }

        memset(new_data, *(int *)val, n*vec->element_size);
        vec->data = new_data;
        vec->capacity = n;
        vec->size = n;
        return;
    }

    // In case size of assign requirement is smaller or equal current capacity
    void* pos = vec->data;
    for(int i = 0; i < n; ++i){
        memcpy(pos, val, vec->element_size);
        pos += vec->element_size;
    }
}

void* vector_front(vector* vec) {
    return vec->data;
}

void* vector_back(vector* vec) {
    return vec->data + (vec->element_size*vec->size);
}

void* vector_data(vector* vec) {
    return vec->data;
}

iterator vector_iterator(vector* vec, int idx){
    iterator ret = {NULL, 0};

    assert(vec != NULL);
    assert(vec->data != NULL);
    assert(idx < vec->size);
    
    if(vec == NULL) return ret;
    if(vec->data == NULL) return ret;
    if(idx < vec->size) return ret;
 
    ret.element_size = vec->element_size;
    ret.ptr = vec->data + idx*vec->element_size;

    return ret;
}

iterator vector_begin(vector* vec){
    return vector_iterator(vec, 0);
}

iterator vector_end(vector* vec){
    return vector_iterator(vec, vec->size);
}