#include "lexer.h"

/*
Do REPL: repeatedly read (load) a line, evaluate (call), and print any result, then loop
*/
void doREPL(){
    char input[256];
    I8 ret;
    while(1){
        printf(">> ");
        fgets(input, 256, stdin);
        
        LexState ls = {
            .line_st = input,
            .current = input,
            .line_num = 0,
        };
        ret = lex(&ls);
        for(int i = 0; i < ls.tok_size; ++i){
            printf("{Type: %d, Data: %s}\n", ls.tok[i].token, ls.tok[i].seminfo);
        }
    }
}

I32 main(I32 argc, char* argv) {
    printf("Interpreter is running...\n");
    doREPL();
    return 0;
}
