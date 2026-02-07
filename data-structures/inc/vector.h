#pragma once
 
#include <stdio.h>
#include "typedef.h"

#define REF_LITERAL(type, val) ((void*)&(type){val})
#define REF(var) ((void*)&var)

#define vector_create(type, size, val) \
    __vector_create(sizeof(type), size, val)

#define vector_get(type, vec, idx) \
    *(type *)__vector_at(vec, idx)

#define vector_size(vec) \
    __vector_size(vec)

#define vector_destroy(vec) \
    __vector_destroy(vec)

#define vector_append(vec, val) \
    __vector_append(vec, val)

#define vector_clear(vec) \
    __vector_clear(vec)

typedef struct{
    void *data;
    size_t size;
    size_t capacity;
    size_t esize;
} vector;

vector __vector_create(size_t esize, size_t size, void *val);
void __vector_destroy(vector *vec);
i32 __vector_append(vector *vec, void *val);
i32 __vector_clear(vector *vec);
void *__vector_at(vector *vec, size_t idx);
size_t __vector_size(vector *vec);
