#include <stdio.h>

int main(int argc, const char *argv[])
{
#ifdef __APPLE__
    printf("MAC\n");
#else
    printf("NO MAC\n");
#endif
    return 0;
}

