#include <stdio.h>
#include "vector.h"

int main() {
    vector vec;
    vector_create(&vec, 5, sizeof(int));
    vector_push_back(&vec, &(int){10});
    vector_push_back(&vec, &(int){20});
    for(int i = 0; i < vec.size; i++) {
        printf("%d\n", ((int*)vec.data)[i]);
    }
    vector_clear(&vec);
    for(int i = 0; i < vec.size; i++) {
        printf("%d\n", ((int*)vec.data)[i]);
    }

    vector_destroy(&vec);
    return 0;
}