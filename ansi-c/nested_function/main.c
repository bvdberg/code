#include <stdio.h>

static void outer(int a) {
    int num = a;

    printf("outer(%d)\n", a);
    void inner(int b) {
        printf("  inner(%d)  num=%d\n", b, num);
    }

    inner(a);
    inner(a*10);

}

int main(int argc, const char *argv[])
{
    outer(10);
    outer(20);
    
    return 0;
}

