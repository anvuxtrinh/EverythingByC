#pragma once

#include <stdio.h>
#include <string.h>
#include "typedef.h"

#ifndef S8_SUCCESS
#define S8_SUCCES 1
#endif

#ifndef S8_FAILURE
#define S8_FAILURE 0
#endif

void *memset(void * ptr, int value, size_t num);
void *memcpy(void * destination, const void * source, size_t num);

typedef struct{
    u8 *str;
    u64 size;
    u64 capacity;
} s8;

#define s8_lit(s) \
    __s8_cptr(s, sizeof(s))

#define s8_create(s) \
    __s8_create(s)

#define s8_assign(s, str) \
    __s8_assign(s, str)

#define s8_string(s) \
    __s8_string(s)

s8 __s8_cptr(char *cptr, u64 size);
s8 __s8_create(s8 *src);
u8 *__s8_string(s8 *s);