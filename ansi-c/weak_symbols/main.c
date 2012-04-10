#include <stdio.h>

void __attribute__ ((weak)) my_function();

int main() {
    if (my_function) {
        printf("running my_function\n");
        my_function();
    } else printf("no my_function found\n");
    //my_function2();
    return 0;
}

