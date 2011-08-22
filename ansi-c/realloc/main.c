#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    void* mem1 = malloc(1024);
    printf("mem1 = %p\n", mem1);

    void* mem3 = malloc(1024);
    printf("mem3 = %p\n", mem3);

    void* mem2 = realloc(mem1, 1024*2);
    printf("mem2 = %p\n", mem2);
    return 0;
}
