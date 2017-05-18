#include <stdio.h>

#define SWAP(a, b) do { a^=b; b^=a; a^=b; } while(0)
#define SWAP_PTR(a, b) do { void* c = a; a = b; b = c; } while(0)

static void test(int a, int b)
{
    printf("%4d  %4d", a, b);
    SWAP(a, b);
    printf("  ->  %4d  %4d\n", a, b);
}

int main(int argc, const char *argv[])
{
    test(10, 20);
    test(10, 10);
    test(-10, 10);
    test(1234, 5678);

    int a = 100;
    int b = 200;
    int* ap = &a;
    int* bp = &b;
    printf("ptr: %p %p   val: %d  %d\n", ap, bp, *ap, *bp);
    SWAP_PTR(ap, bp);
    printf("ptr: %p %p   val: %d  %d\n", ap, bp, *ap, *bp);

    return 0;
}

