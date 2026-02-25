 #pragma once

#include <stdio.h>

#include "vector.h"
#include "typedef.h"

#ifndef S8_SUCCESS
#define S8_SUCCESS 1
#endif

#ifndef S8_FAILURE
#define S8_FAILURE 0
#endif

void *memset(void * ptr, int value, size_t num);
void *memcpy(void * destination, const void * source, size_t num);
size_t strlen (const char *str);

typedef struct{
    u8 *str;
    u64 size;
    u64 len;
    u64 capacity;
} s8;

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

s8 __s8_create_by_cptr(char *cptr, u64 size);
s8 __s8_create_by_another(s8 *src);
s8 __s8_create();
u8 *__s8_string(s8 *s);
u64 __s8_size(s8 *s);
u64 __s8_len(s8 *s);
u8 *__s8_at(s8 *s, u64 idx);
char __s8_char(s8 *s, u64 idx);
void __s8_destroy(s8 *s);
u8 __s8_substr(s8 *sub, s8 *src, u64 st, u64 en);
u8 __s8_append(s8 *s, s8 *value);
u8 __s8_append_char(s8 *s, char c);
u8 __s8_split(vector *sub, s8 *s, char seperator);
