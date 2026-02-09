#include "typedef.h"
#include "vector.h"
#include "lexer.h"
#include "parser.h"

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
    char input[256];
    i8 ret;
    while(1){
        printf(">> ");
        fgets(input, 256, stdin);
        
        LexState ls = {
            .tok = vector_create(Token, 0, NULL),
            .line_st = input,
            .current = input,
            .line_num = 0,
        };
        ret = lex(&ls);
        for(int i = 0; i < vector_size(&(ls.tok)); ++i){
            Token *buf = vector_at(Token, &(ls.tok), i);
            printf("{Type: %d, Data: %s}\n", buf->token, buf->seminfo);
        }

        // Parser parser = parser_init(&ls);
        BinExpr *ast = parse(&ls);

        vector expr_list = vector_create(Expr, 0, NULL);
        bton(&expr_list, ast);

        for(u64 i = 0; i < vector_size(&expr_list); ++i){
            Expr *ex = vector_at(Expr, &expr_list, i);
            printf("{%d - %s} ", ex->type, ex->data);
        }
        cleanup(&ls);
    }
}

i32 main(i32 argc, char* argv) {
    printf("Interpreter is running...\n");
    doREPL();
    return 0;
}
