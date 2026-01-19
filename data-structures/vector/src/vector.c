#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "vector.h"

int __realloc(vector* vec, size_t new_size){
    assert(vec != NULL);

    if(vec == NULL) return VECTOR_FAILURE;

    void* new_data = realloc(vec->data, new_size * vec->element_size);
    if(new_data == NULL){
        return VECTOR_FAILURE;
    }

    vec->data = new_data;
    vec->capacity = new_size;

    return VECTOR_SUCCESS;
}

bool __should_grow(vector* vec){
    assert(vec != NULL);

    if(vec == NULL) return false;

    return vec->size >= vec->capacity;
}

bool __should_shrink(vector* vec){
    assert(vec != NULL);

    if(vec == NULL) return false;

    return vec->size < vec->capacity;
}

int __adjust_capacity(vector* vec){
    assert(vec != NULL);

    if(vec == NULL) return VECTOR_FAILURE;

    if(__realloc(vec, vec->capacity * 2) == VECTOR_FAILURE){
        return VECTOR_FAILURE;
    }

    return VECTOR_SUCCESS;
}

int __update_element(vector* vec, size_t idx, void* value){
    assert(vec != NULL);
    assert(value != NULL);

    if(vec == NULL) return VECTOR_FAILURE;
    if(value == NULL) return VECTOR_FAILURE;

    void* dict = vector_at(vec, idx);
    memcpy(dict, value, vec->element_size);
    return VECTOR_SUCCESS;
}

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
    if(__should_grow(vec)){
        if(__adjust_capacity(vec) == VECTOR_FAILURE){
            return VECTOR_FAILURE;
        }
    }

    //Copy the element to the end of the vector
    __update_element(vec, vec->size, element);
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

int vector_assign(vector* vec, size_t n, void* val) {
    assert(vec != NULL);
    assert(val != NULL);

    if(vec == NULL) return VECTOR_FAILURE;
    if(val == NULL) return VECTOR_FAILURE;
    
    // In case size of assign requirement is greater current capacity
    if(vec->capacity < n){
        if(__realloc(vec, n) == VECTOR_FAILURE){
            return VECTOR_FAILURE;
        }
        memset(vec->data, *(int *)val, n*vec->element_size);
        vec->size = vec->capacity;
        return VECTOR_SUCCESS;
    }

    // In case size of assign requirement is smaller or equal current capacity
    void* pos = vec->data;
    for(int i = 0; i < n; ++i){
        __update_element(vec, i, val);
    }
    return VECTOR_SUCCESS;
}

void* vector_front(vector* vec) {
    return vec->data;
}

void* vector_back(vector* vec) {
    return vector_at(vec, vec->size);
}

void* vector_data(vector* vec) {
    return vec->data;
}

int vector_shrink_to_fit(vector* vec){
    assert(vec != NULL);

    if(vec == NULL) return VECTOR_FAILURE;

    if(__should_shrink(vec)){
        if(__realloc(vec, vec->size) == VECTOR_FAILURE){
            return VECTOR_FAILURE;
        }
    }

    return VECTOR_SUCCESS;
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

bool vector_empty(vector* vec){
    return vec->size == 0;
}

size_t vector_size(vector* vec){
    return vec->size;
}

int vector_resize(vector* vec, size_t new_size, void* val){
    assert(vec != NULL);

    if(vec != NULL) return VECTOR_FAILURE;

    int __old = vec->size;
    vec->size = new_size;

    if(new_size > vec->capacity){
        __realloc(vec, new_size);
    }

    if(val != NULL){
        void* __end = vector_at(vec, __old);
        for(int i = 0; i < new_size - __old; ++i){
            memset(__end + i, *(int *)val, vec->element_size);
        }
    }

    return VECTOR_SUCCESS;
}

