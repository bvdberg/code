// strange aray initialization. Compile with gcc main.c

#include <stdio.h>

#define set_priv(x,f) [x - 1] = f

static const int bla[] = {
    set_priv(5, 2),
    set_priv(6, 3),
    [2] = 7,
    set_priv(10, 14),
    set_priv(1, 43514),
};

static int bla2[10] = { 0 };

int main() {
    unsigned int i;
    printf("size = %d\n", sizeof(bla)/sizeof(int));
    for (i=0; i<sizeof(bla)/sizeof(int); i++) {
        printf("array[%d] = %d\n", i, bla[i]);
    }

    for (i=0; i<sizeof(bla2)/sizeof(int); i++) {
        printf("array2[%d] = %d\n", i, bla2[i]);
    }
    return 0;
}

