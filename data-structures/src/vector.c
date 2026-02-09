#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

#ifndef VECTOR_SUCCESS
#define VECTOR_SUCCESS 1
#endif

#ifndef VECTOR_FAILURE
#define VECTOR_FAILURE 0
#endif

void *__init_dynamic_array(u64 size);
i32 __update_element(vector *vec, u64 idx, void *val);

u8 __is_initialized(vector *vec){
    return vec != NULL && vec->data != NULL;
}

u8 __valid_index(vector *vec, u64 idx){
    return idx < vec->size;
}

u8 __valid_ptr(void *val){
    return val != NULL;
}

u8 __should_grow(vector *vec){
    return vec->size >= vec->capacity;
}

i32 __adjust_capacity(vector *vec){
    assert(__is_initialized(vec));

    if(!__is_initialized(vec)){ return VECTOR_FAILURE; }

    vec->capacity = vec->capacity == 0 ? 1 : 2*vec->capacity;
    void *reallocated = realloc(vec->data, vec->esize*vec->capacity);
    if(!reallocated){
        return VECTOR_FAILURE;
    }
    vec->data = reallocated;
    return VECTOR_SUCCESS;
}

i32 __push_back(vector *vec, void *val){
    assert(__is_initialized(vec));
    assert(__valid_ptr(val));

    if(!__is_initialized(vec)){ return VECTOR_FAILURE; }
    if(!__valid_ptr(val)){ return VECTOR_FAILURE; }

    if(vec->size >= vec->capacity){ return VECTOR_FAILURE; }

    u64 size = vec->esize*vec->size;
    void *dict = (char *) vec->data + size;
    memcpy(dict, val, vec->esize);
    ++vec->size;
    return VECTOR_SUCCESS;
}

void *__init_dynamic_array(u64 size){
    void *array = malloc(size);
    if(array == NULL){ return NULL; }
    memset(array, 0, size);
    return array;
}

i32 __update_element(vector *vec, u64 idx, void *val){
    assert(__is_initialized(vec));
    assert(__valid_ptr(val));

    if(!__is_initialized(vec)) { return VECTOR_FAILURE; }
    if(!__valid_ptr(val)) { return VECTOR_FAILURE; }
    
    void *dict = __vector_at(vec, idx);
    if(!__valid_ptr(dict)) { return VECTOR_FAILURE; }
    memcpy(dict, val, vec->esize);

    return VECTOR_SUCCESS;
}

vector __vector_create(u64 esize, u64 _enum, void *val){
    u64 size = _enum * esize;
    vector vec;
    
    vec.data = __init_dynamic_array(size);
    if(!__valid_ptr(vec.data)) { return vec; }

    vec.esize = esize;
    vec.capacity = _enum;
    vec.size = _enum;

    if(!__valid_ptr(val)){ return vec; }

    //Initialize value
    for(u64 i = 0; i < _enum; ++i){
        __update_element(&vec, i, val);  
    }

    return vec;
}

void *__vector_at(vector *vec, u64 idx){
    assert(__is_initialized(vec));
    assert(__valid_index(vec, idx));

    if(!__is_initialized(vec)) { return NULL; }
    if(!__valid_index(vec, idx)) { return NULL; }

    return (char *)vec->data + idx*vec->esize;
}

u64 __vector_size(vector *vec){
    assert(vec != NULL);

    if(vec == NULL){ return 0; }

    return vec->size;
}

void __vector_destroy(vector *vec){
    assert(vec != NULL);

    if(vec == NULL){ return; }

    if(vec->data != NULL){
        free(vec->data);
        vec->data = NULL;
    }
    vec->esize = 0;
    vec->size = 0;
    vec->capacity = 0;
}

i32 __vector_append(vector *vec, void *val){
    assert(__is_initialized(vec));
    assert(__valid_ptr(val));

    if(!__is_initialized(vec)){ return VECTOR_FAILURE; }
    if(!__valid_ptr(val)) { return VECTOR_FAILURE; }

    if(__should_grow(vec)){
        __adjust_capacity(vec);
    }

    return __push_back(vec, val);
}

i32 __vector_clear(vector *vec){
    assert(__is_initialized(vec));

    if(!__is_initialized(vec)){ return VECTOR_FAILURE; }

    memset(vec->data, 0x00, vec->esize * vec->size);
    vec->size = 0;
    return VECTOR_SUCCESS;
}