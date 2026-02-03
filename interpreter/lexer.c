#include "lexer.h"

I8 push_tok(LexState *ls, TokenType tok_type, char *sem);
I8 isdigit(unsigned char c);
I8 stoi(char *c, size_t len);
U8 get_digit(char *line, U8 idx);

I8 lex(LexState *ls){
    char c;
    while((c = *ls->current) != '\0'){
        switch(c){
            case '+':
                push_tok(ls, TK_PLUS, "+");
                break;
            default:
                if(isdigit(c)){
                    char *st = ls->current;
                    while(c != '\0' && isdigit(c)){ c = *(++ls->current); }
                    U8 len = ls->current - st;
                    char *digit = (char *)malloc(sizeof(char)*len + 1);
                    memcpy(digit, st, sizeof(char)*len);
                    push_tok(ls, TK_INT, digit);
                    continue;
                }
                break;
        }
        ++ls->current;
    }
}

U8 get_digit(char *line, U8 idx){
    while(isdigit(line[idx])){

    }
}

I8 push_tok(LexState *ls, TokenType tok_type, char *sem){
    Token *tok = ls->tok;
    size_t capacity = ls->tok_capacity;
    size_t size = ls->tok_size;

    //Allocate memory
    if(capacity >= size){
        if(capacity == 0){
            tok = (Token *)malloc(1 * sizeof(Token));
            capacity = 1;
        }else{
            size_t new_capacity = capacity * 2;
            Token *new_tok = (Token *)realloc(tok, sizeof(Token) * new_capacity);
            tok = new_tok;
            capacity = new_capacity;
        }
    }
    
    //Update new infomation

    tok[size].token = tok_type;
    tok[size].seminfo = sem;
    ls->tok = tok;
    ls->tok_capacity = capacity;
    ls->tok_size = size + 1;
    return 1;
}

I8 isdigit(unsigned char c){
    I8 num = c - '0';
    return num >= 0 && num < 10;
}

I8 stoi(char *c, size_t len){
    return 1;
}
