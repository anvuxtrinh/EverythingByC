#include "typedef.h"
#include "vector.h"
#include "string.h"
#include "lexer.h"
#include "parser.h"

#define MAX_INPUT_LEN 256
/*
Do REPL: repeatedly read (load) a line, evaluate (call), and print any result, then loop
*/

void bton(vector *nums, BinExpr *root){
    if(root == NULL){
        return;
    }
    vector_append(nums, &root->op);
    bton(nums, root->lhs);
    bton(nums, root->rhs);
}
    
void doREPL(){
    char input[MAX_INPUT_LEN];
    i8 ret;
    while(1){
        printf(">> ");
        fgets(input, MAX_INPUT_LEN, stdin);

        s8 text = s8_create(input, strlen(input));
        Lexer l = {
            .tok = vector_create(Token, 0, NULL),
            .text = text,
            .bol = s8_at(&text, 0),
            .cursor = s8_at(&text, 0)
        };
        ret = lex(&l);
        for(int i = 0; i < vector_size(&(l.tok)); ++i){
            Token *buf = vector_at(Token, &(l.tok), i);
            printf("{Type: %d, Data: %s}\n", buf->token, s8_string(&buf->seminfo));
        }

        // Parser parser = parser_init(&ls);
        BinExpr *ast = parse(&l);

        vector expr_list = vector_create(Expr, 0, NULL);
        bton(&expr_list, ast);

        for(u64 i = 0; i < vector_size(&expr_list); ++i){
            Expr *ex = vector_at(Expr, &expr_list, i);
            printf("{%d - %s} ", ex->type, s8_string(&ex->data));
        }
        cleanup(&l);
    }
}

i32 main(i32 argc, char* argv) {
    printf("Interpreter is running...\n");
    doREPL();
    return 0;
}
