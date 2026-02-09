#include "lexer.h"

i8 push_tok(LexState *ls, TokenType tok_type, char *sem);
i8 isdigit(unsigned char c);
i8 stoi(char *c, size_t len);
u8 get_digit(char *line, u8 idx);

i8 lex(LexState *ls){
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
                    u8 len = ls->current - st;
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

i8 push_tok(LexState *ls, TokenType tok_type, char *sem){
    Token tok = { .token = tok_type, .seminfo = sem };
    vector_append(&(ls->tok), REF(tok));
    return 1;
}

i8 isdigit(unsigned char c){
    i8 num = c - '0';
    return num >= 0 && num < 10;
}

void cleanup(LexState *ls){
    for(i32 idx = 0; idx < ls->tok_size; ++idx){
        Token *tok = vector_at(Token, &(ls->tok), idx);
        if(tok->token == TK_INT) { 
            free(tok->seminfo); 
            tok->seminfo = NULL;
        }

    }
    vector_destroy(&(ls->tok));
}
