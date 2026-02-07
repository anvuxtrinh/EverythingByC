#include "inc/typedef.h"
#include "inc/vector.h"

i32 main(i32 argc, char *argv[]){
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
    return 0;
}

