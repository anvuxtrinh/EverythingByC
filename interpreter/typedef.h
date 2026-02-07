#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*Integer define*/
typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint8_t     u8;
typedef int64_t     i64;
typedef int32_t     i32;
typedef int8_t      i8;

/*vector*/
typedef struct{
    void *data;
    size_t size;
    size_t capacity;
    size_t esize;
} vector;

/*Lex*/
#define FIRST_RESERVED 256

typedef enum {
    /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED,
  TK_FLT, TK_INT, TK_NAME, TK_STRING, TK_PLUS, TK_EOF,
} TokenType;

typedef union {
  i8 integer;
  char *str;
} Sematics;

typedef struct {
  TokenType token;
  char* seminfo;
}Token;

typedef struct {
  vector tok;
  size_t tok_size;
  size_t tok_capacity;
  char *line_st;
  char *current;
  u8 line_num;
} LexState;

/*Parser*/
typedef struct {
  vector tok;
  u32 tok_idx;
}Parser;

/*Ast*/
typedef enum {
    ADD_SUB,
    DIV_MUL
} Precedence;

typedef enum {
    ATOM,
    OPERATION
} ExprType;

typedef struct {
    ExprType type;
    char *data;
} Expr;

typedef struct{
    Expr rhs;
    Expr lhs;
    Expr op;
}BinaryExpr;
