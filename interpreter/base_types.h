#pragma once

#include <stdint.h>

/*Integer define*/
typedef uint64_t    U64;
typedef uint32_t    U32;
typedef uint8_t     U8;
typedef int64_t     I64;
typedef int32_t     I32;
typedef int8_t      I8;

/*String define*/
typedef struct {
    U8* data_ptr;
    U64 size;
} S8;

#define STR_INIT(s) { .data_ptr = (U8*) s, .size = sizeof(s) - 1}
#define STR_FMT(s) (U8*) s.data_ptr
#define STR_SIZE(s) (U64) s.size

/*Lex*/
#define FIRST_RESERVED 256

enum RESERVED {
    /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED,
  TK_FLT, TK_INT, TK_NAME, TK_STRING
};

typedef union {
    I8 integer;
    S8 str;
} Sematics;

typedef struct {
    int token;
    Sematics seminfo;
}Token;
