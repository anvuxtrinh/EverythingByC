#include "lexer.h"

I8 push_tok(LexState *ls, TokenType tok_type, char *sem, size_t sem_size);
I8 _isdigit(char c);
I8 stoi(char *c, size_t len);

I8 lex(LexState *ls){
    char c; U8 idx = 0;
    char *line = ls->line;
    
    while((c = line[idx]) != '\0'){
        push_tok(ls, TK_PLUS, &c, 1);
        // switch (c)
        // {
        // case '+':
        //     push_tok(ls, TK_PLUS, "+");
        //     break;
        
        // default:
        //     char buff[256];
        //     U8 i = 0;
        //     while(isdigit(c)){
        //         if(i == 256){ break; }
        //         buff[i] = c;
        //         ++i;
        //     }
        //     if(i != 0){ push_tok(ls, TK_INT, buff); }
        //     break;
        // }
        idx++;
    }
}

I8 push_tok(LexState *ls, TokenType tok_type, char *sem, size_t sem_size){
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

I8 _isdigit(char c){
    return c - '0' >= 0 &&  c - '0' < 10;
}

I8 stoi(char *c, size_t len){
    return 1;
}
