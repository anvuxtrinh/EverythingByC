#include "inc/typedef.h"
#include "inc/vector.h"
#include "inc/string.h"

i32 main(i32 argc, char *argv[]){
    //vector
    i32 a = 5;
    vector vec = vector_create(i32, 4, REF(a));
    vector_append(&vec, REF_LITERAL(i32, 9));
    vector_append(&vec, REF(a));
    vector_clear(&vec);
    for(int i = 0; i < vector_size(&vec); ++i){
        printf("%d ", vector_get(i32, &vec, i));
    }
    printf("\n");
    vector_destroy(&vec);

    //string
    s8 s = s8_lit("Hello Wold");
    s8 empty = s8_create(NULL);
    s8 cp = s8_create(&s);

    printf("%s\n", s8_string(&s));
    printf("%s\n", s8_string(&empty));
    printf("%s\n", s8_string(&cp));
    return 0;
}

