#include <stdlib.h>
#include <assert.h>
#include "string.h"

s8 __s8_create(s8 *src){
    s8 s;
    if(src == NULL){
        s.str = malloc(1);
        s.size = 0;
        s.capacity = 1;
        memset(s.str, 0x00, 1);
    }else{
        s.str = malloc(src->size + 1);
        s.size = src->size;
        s.capacity = src->size + 1;
        memcpy(s.str, src->str, s.size);
    }
    return s;
}

s8 __s8_cptr(char *cptr, u64 size){
    assert(cptr != NULL);
    
    s8 s = {
        .str = NULL,
        .size = 0,
        .capacity = 0
    };

    if(cptr == NULL){ return s; }

    s.str = malloc(size);
    s.size = size;
    s.capacity = size;
    memcpy(s.str, cptr, size);
    return s;
}

u8 *__s8_string(s8 *s){
    return s->str; 
}