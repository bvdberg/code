/*
 * g++ main.cpp -pg -o example1 -O2 -lc
 * ./example
 * gprof example1 gmon.out -p
 *
 * or compile with -g and use gprof -A to get annotated source
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int a(void) {
    int i=0,g=0;
    while(i++<100000)
    {
        g+=i;
    }
    return g;
}

int b(void) {
    int i=0,g=0;
    while(i++<400000)
    {
        g+=i;
    }
    return g;
}

int main(int argc, char** argv)
{
    int iterations;

    if(argc != 2)
    {
        printf("Usage %s <No of Iterations>\n", argv[0]);
        exit(-1);
    }
    else
        iterations = atoi(argv[1]);

    printf("No of iterations = %d\n", iterations);

    while(iterations--)
    {
        a();
        b();
    }
}
