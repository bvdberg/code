#include <stdio.h>
#include <unistd.h>

void testfunc()
{
#ifdef WHILE
    while(1) {
        printf("hallo\n");
        sleep(1);
    }
#endif
#ifdef FOR
    for(;;) {
        printf("hallo\n");
        sleep(1);
    }
#endif
}

int main(int argc, const char *argv[])
{
    testfunc();
    return 0;
}

