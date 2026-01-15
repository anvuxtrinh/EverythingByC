#include "../../typedef.h"
#include <stdio.h>

typedef struct {
    u8* data_ptr;
    u64 size;
} string;

#define STR_INIT(s) { .data_ptr = (u8*) s, .size = sizeof(s) - 1}
#define STR_FMT(s) (u8*) s.data_ptr
#define STR_SIZE(s) (u64) s.size

string substr(string __str, u64 __st, u64 __en);

int main(){
    string test = STR_INIT("Hello World");
    string substr_test = substr(test, 1, 3);
    printf("%.*s\n", STR_SIZE(substr_test), STR_FMT(substr_test));
    return 0;
}

string substr(string __str, u64 __st, u64 __en){
    return (string){ .data_ptr = __str.data_ptr + __st, .size = __en - __st };
}