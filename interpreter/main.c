#include <stdio.h>
#include <stdint.h>

typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint8_t     u8;
typedef int64_t     i64;
typedef int32_t     i32;
typedef int8_t      i8;

typedef struct {
    u8* data_ptr;
    u64 size;
} string;

#define STR_INIT(s) { .data_ptr = (u8*) s, .size = sizeof(s) - 1}
#define STR_FMT(s) (u8*) s.data_ptr
#define STR_SIZE(s) (u64) s.size

i32 main(i32 argc, char* argv) {
    char s[1000];
    printf("Interpreter is running...\n");
    while(1){
        printf(">> ");
        fgets(s, 1000, stdin);
        printf("%s\n", s);
    }
    return 0;
}