#include "lexer.h"
#include "string.h"

i8 push_tok(Lexer *l, TokenType tok_type, s8 sem);
i8 isdigit(unsigned char c);
i8 stoi(char *c, size_t len);
u8 get_digit(char *line, u8 idx);

i8 lex(Lexer *l){
    char c;
    while((c = *l->cursor) != '\0'){
        switch(c){
            case '+':
                push_tok(l, TK_PLUS, s8_create("+", strlen("+")));
                break;
            default:
                if(isdigit(c)){
                    u64 st = l->cursor - l->bol;
                    while(c != '\0' && isdigit(c)){ c = *(++l->cursor); }
                    u64 en = l->cursor - l->bol;
                    s8 digit = s8_substr(&l->text, st, en);
                    push_tok(l, TK_INT, digit);
                    continue;
                }
                break;
        }
        ++l->cursor;
    }
    
}

i8 push_tok(Lexer *l, TokenType tok_type, s8 sem){
    Token tok = { .token = tok_type, .seminfo = sem };
    vector_append(&(l->tok), REF(tok));
    return 1;
}

i8 isdigit(unsigned char c){
    i8 num = c - '0';
    return num >= 0 && num < 10;
}

void cleanup(Lexer *l){
    for(i32 idx = 0; idx < vector_size(&l->tok); ++idx){
        Token *tok = vector_at(Token, &(l->tok), idx);
        s8_destroy(&tok->seminfo);
    }
    vector_destroy(&(l->tok));
}
