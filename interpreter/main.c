#include "typedef.h"
#include "vector.h"
#include "lexer.h"
#include "parser.h"

/*
Do REPL: repeatedly read (load) a line, evaluate (call), and print any result, then loop
*/
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
        BinaryExpr ast = parse(&ls);

        printf("{%d, %s}, {%d, %s}, {%d, %s}\n", ast.lhs.type, ast.lhs.data, ast.op.type, ast.op.data, ast.rhs.type, ast.rhs.data);

        cleanup(&ls);
    }
}

i32 main(i32 argc, char* argv) {
    printf("Interpreter is running...\n");
    doREPL();
    return 0;
}
