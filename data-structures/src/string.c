#include <stdlib.h>
#include <assert.h>
#include "string.h"

s8 __s8_create_by_another(s8 *src){
    if(src == NULL){
        return __s8_create();
    }
    
    s8 s;
    s.str = (u8 *)malloc(src->size);
    if(s.str == NULL) { return s; }
    s.len = src->len;
    s.size = src->size;
    s.capacity = src->size;
    memcpy(s.str, src->str, s.size);

    return s;
}

s8 __s8_create_by_cptr(char *cptr, u64 len){
    assert(cptr != NULL);
    
    s8 s = {
        .str = NULL,
        .size = 0,
        .capacity = 0
    };

    if(cptr == NULL){ return s; }

    s.str = (u8 *)malloc(len + 1);
    s.len = len;
    s.size = len + 1;
    s.capacity = len + 1;
    memcpy(s.str, cptr, s.size);
    return s;
}

s8 __s8_create(){
    s8 s;
    s.str = (u8 *)malloc(1);
    if(s.str == NULL) { return s; }
    s.len = 0;
    s.size = 1;
    s.capacity = 1;
    memset(s.str, 0x00, 1);
    return s;
}

u8 *__s8_string(s8 *s){
    return s->str; 
}

u64 __s8_size(s8 *s){
    return s->size;
}

u64 __s8_len(s8 *s){
    return s->len;
}

u8 *__s8_at(s8 *s, u64 idx){
    return idx > s->len ? NULL : s->str + idx;
}

char __s8_char(s8 *s, u64 idx){
    return idx > s->len ? '\0' : s->str[idx];
}

void __s8_destroy(s8 *s){
    if(s->str != NULL){
        free(s->str);
        s->str = NULL;
    }
    s->len = 0;
    s->size = 0;
    s->capacity = 0;
}

s8 __s8_substr(s8 *s, u64 st, u64 en){
    s8 sub;
    u64 len = en > s->len ? s->len - st : en - st;
    sub.str = (u8 *)malloc(len + 1);
    sub.len = len;
    sub.size = len + 1;
    sub.capacity = len + 1;
    memcpy(sub.str, s->str + st, sub.size);
    return sub;
}
