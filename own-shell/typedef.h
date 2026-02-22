#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*Integer define*/
typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint8_t     u8;
typedef int64_t     i64;
typedef int32_t     i32;
typedef int8_t      i8;
typedef bool        b8;

/*vector*/
typedef struct{
    void *data;
    size_t size;
    size_t capacity;
    size_t esize;
} vector;

/*string*/
typedef struct{
    u8 *str;
    u64 size;
    u64 len;
    u64 capacity;
} s8;

#define MIN(a, b) a < b ? a : b 