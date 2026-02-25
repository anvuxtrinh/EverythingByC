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

static u64 __get_new_capacity(u64 old_capacity){
    return old_capacity * 2;
}

static b8 __should_grow(s8 *s){
    return s->size >= s->capacity;
}

static u8 __adjust_capacity(s8 *s, u64 new_capacity){
    if(new_capacity <= s->capacity) {
        return S8_FAILURE;
    }
    u8 *new_str = (u8 *)realloc(s->str, new_capacity);
    if(new_str == NULL) {
        return S8_FAILURE;
    }
    s->str = new_str;
    s->capacity = new_capacity;
    return S8_SUCCESS;
}

u8 __s8_append(s8 *s, s8 *value){
    if(s == NULL || value == NULL) { return S8_FAILURE; }

    u64 new_len = s->len + value->len;
    u64 new_capacity = s->capacity;
    b8 should_grow = false;
    while(new_capacity <= new_len) {
        should_grow = true;
        new_capacity = __get_new_capacity(new_capacity);
    }
    if(should_grow) {
        if(__adjust_capacity(s, new_capacity) == S8_FAILURE) {
            return S8_FAILURE;
        }
    }
    memcpy(s->str + s->len, value->str, value->len);
    s->len = new_len;
    s->size = new_len + 1;
    s->str[s->len] = '\0';
    return S8_SUCCESS;
}

u8 __s8_append_char(s8 *s, char c){
    if(s == NULL) {
        return S8_FAILURE;
    }
    if(__should_grow(s))
    {
        u64 new_capacity = __get_new_capacity(s->capacity);
        if(__adjust_capacity(s, new_capacity) == S8_FAILURE) {
            return S8_FAILURE;
        }
    }
    s->str[s->len] = c;
    ++s->len;
    ++s->size;
    s->str[s->len] = '\0';
    return S8_SUCCESS;
}

u8 __s8_split(vector *sub, s8 *s, char seperator){
    return S8_SUCCESS;
}

u8 __s8_substr(s8 *sub, s8 *src, u64 st, u64 en){
    return S8_SUCCESS;
}
