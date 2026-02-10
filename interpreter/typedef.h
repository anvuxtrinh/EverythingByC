#pragma once

#include <stdlib.h>
#include <stdint.h>

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

/*string*/
typedef struct{
    u8 *str;
    u64 size;
    u64 len;
    u64 capacity;
} s8;

/*Lex*/
#define FIRST_RESERVED 256

typedef enum {
    /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED,
  TK_FLT, TK_INT, TK_NAME, TK_STRING, TK_PLUS, TK_EOF,
} TokenType;

typedef union {
  i8 integer;
  s8 str;
} Sematics;

typedef struct {
  TokenType token;
  s8 seminfo;
}Token;

typedef struct {
  vector tok;
  s8 text;
  const char *bol;  /*begin of line*/
  const char *cursor;
} Lexer;

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
    s8 data;
} Expr;

typedef struct binary_expr{
    struct binary_expr *rhs;
    struct binary_expr *lhs;
    Expr op;
}BinExpr;
