#include <stdio.h>
#include "base_types.h"

I32 main(I32 argc, char* argv) {
    S8 s;
    printf("Interpreter is running...\n");
    while(1){
        printf(">> ");
        fgets(s.data_ptr, 256, stdin);
        printf("%s\n", STR_FMT(s));
    }
    return 0;
}