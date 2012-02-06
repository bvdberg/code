#include <stdio.h>
#include <alloca.h>
#include <stdlib.h>

static void myfunc(int num) {
    void* buf = alloca(num);
    printf("buf = %p\n", buf);
}

int main(int argc, const char *argv[])
{
    int num = atoi(argv[1]);
    myfunc(num);    
    return 0;
}

