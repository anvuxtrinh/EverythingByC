#include "lexer.h"

I8 push_tok(LexState *ls, TokenType tok_type, char *sem);
I8 isdigit(char c);
I8 stoi(char *c, size_t len);

I8 lex(LexState *ls){
    char c; U8 idx = 0;
    char *line = ls->line;
    Token *tok = ls->tok;
    size_t tok_cnt = ls->tok_cnt;
    
    while(c = line[idx] != '\0'){
        switch (c)
        {
        case '+':
            push_tok(ls, TK_PLUS, "+");
            break;
        
        default:
            char buff[256];
            U8 i = 0;
            while(isdigit(c)){
                if(i == 256){ break; }
                buff[i] = c;
                ++i;
            }
            if(i != 0){ push_tok(ls, TK_INT, buff); }
            break;
        }
        idx++;
    }
}

I8 push_tok(LexState *ls, TokenType tok_type, char *sem){
    return 1;
}

I8 isdigit(char c){
    return 1;
}

I8 stoi(char *c, size_t len){
    return 1;
}
