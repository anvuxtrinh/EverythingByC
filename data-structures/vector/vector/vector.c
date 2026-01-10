#include "vector.h"

vector* vector_create(size_t element_size) {
    vector* vec = (vector*)calloc(sizeof(vector));
    if (!vec) {
        return NULL;
    }
    vec->data = malloc(element_size);
    if (!vec->data) {
        free(vec);
        return NULL;
    }
    vec->size = element_size;
    vec->capacity = element_size;
    return vec;
}

int vector_push_back(vector* vec, void* element, size_t element_size){
    if(vec->size + element_size > vec->capacity){
        size_t new_capacity = vec->capacity * 2;
        void* new_data = realloc(vec->data, new_capacity);
        if(!new_data){
            return -1; // reallocation failed
        }
        vec->data = new_data;
        vec->capacity = new_capacity;
    }
    memcpy((char*)vec->data + vec->size, element, element_size);
    vec->size += element_size;
    return 0;
}

int vector_clear(vector* vec){
    if(!vec){
        return -1;
    }

    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    free(vec);
    vec = NULL;
    return 0;
}