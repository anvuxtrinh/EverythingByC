#pragma once

#include <stdlib.h>
#include <assert.h>
#include "typedef.h"
#include "vector.h"

#ifndef S8_SUCCESS
#define S8_SUCCESS 1
#endif

#ifndef S8_FAILURE
#define S8_FAILURE 0
#endif

#define S8_DISPATCH(_1, _2, _3, NAME, ...) NAME
#define s8_create(...) \
    S8_DISPATCH(dummy, ##__VA_ARGS__, __s8_create_by_cptr, __s8_create_by_another, __s8_create)(__VA_ARGS__)

#define s8_assign(s, str) \
    __s8_assign(s, str)

#define s8_string(s) \
    __s8_string(s)

#define s8_size(s) \
    __s8_size(s)

#define s8_len(s) \
    __s8_len(s)

#define s8_at(s, idx) \
    (char *)__s8_at(s, idx)

#define s8_char(s, idx) \
    __s8_char(s, idx)

#define s8_destroy(s) \
    __s8_destroy(s)

#define s8_substr(s, st, en) \
    __s8_substr(s, st, en)

#define s8_strcat(s1, s2) \
    __s8_strcat(s1, s2)

#define s8_append(s, c) \
    __s8_append(s, c)

#define s8_find(s, st, sub) \
    __s8_find(s, st, sub)

#define s8_find_all(s, sub) \
    __s8_find_all(s, sub)

#define s8_erase(s, idx, len) \
    __s8_erase(s, idx, len)

s8 __s8_create_by_cptr(char *cptr, u64 size);
s8 __s8_create_by_another(s8 *src);
s8 __s8_create();
u8 *__s8_string(s8 *s);
u64 __s8_size(s8 *s);
u64 __s8_len(s8 *s);
u8 *__s8_at(s8 *s, u64 idx);
char __s8_char(s8 *s, u64 idx);
void __s8_destroy(s8 *s);
s8 __s8_substr(s8 *s, u64 st, u64 en);
s8 __s8_strcat(s8 *s1, s8 *s2);
u8 __s8_append(s8 *s, char c);
i64 __s8_find(s8 *s, u64 st, char *sub);
vector __s8_find_all(s8 *s, char *sub);
void __s8_replace(s8 *s, char *old, char *new);
void __s8_erase(s8 *s, u64 idx, u64 len);

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
    memcpy(sub.str, s->str + st, len);
    sub.str[len] = '\0';
    return sub;
}

s8 __s8_strcat(s8 *s1, s8 *s2){
    s8 ret;

    ret.len = s1->len + s2->len;
    ret.size = ret.len + 1;
    ret.capacity = ret.size;

    ret.str = (u8 *)malloc(ret.capacity);
    if(ret.str == NULL) { return ret; }
    memcpy(ret.str, s1->str, s1->len);
    memcpy(ret.str + s1->len, s2->str, s2->size);
    return ret;
}

u8 __s8_append(s8 *s, char c){
    if(s->size >= s->capacity){
        s->capacity *= 2;
        u8 *another = realloc(s->str, s->capacity);
        if(!another){
            return S8_FAILURE;
        }
        s->str = another;
    }
    memcpy((void*)(s->str + s->len), (void *)&c, sizeof(u8));
    ++s->size;
    ++s->len;
    s->str[s->size] = '/0';
    return S8_SUCCESS;
}

u64 __strlen(char* str){
    u64 len = 0;
    while(str[len] != '\0'){
        ++len;
    }
    return len;
}

i64 __s8_find(s8 *s, u64 st, char *sub){
    u64 len1 = s->len;
    u64 len2 = __strlen(sub);

    for(u64 i = st; i < len1-len2; ++i){
        u64 j = 0;
        while(j < len2 && s->str[i+j] == sub[j]){
            ++j;
        }
        if(j == len2){
            return i;
        }
    }

    return -1;
}

vector __s8_find_all(s8 *s, char *sub){
    u64 st = 0;
    u64 sub_len = __strlen(sub);
    vector ret = vector_create(u64, 0, NULL);
    i64 idx;
    while((idx = __s8_find(s, st, sub)) != -1){
        vector_append(&ret, (void *)&idx);
        st = idx + sub_len;
    }
    return ret;
}

void __s8_replace(s8 *s, char *old, char *new){

}

void __s8_erase(s8 *s, u64 idx, u64 len){
    u64 copy_st = idx + len;
    u64 copy_len = s->len - copy_st;
    memcpy(s->str + idx, s->str + copy_st, copy_len);
    s->str[idx + copy_len] = '\0';
    s->len -= len;
    s->size -= len;
}
