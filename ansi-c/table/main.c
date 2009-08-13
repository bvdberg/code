#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    if (argc < 2) return -1;
    
    int base = atoi(argv[1]);
    int times = 10;
    if (argc == 3) times = atoi(argv[2]);
    int i;
    for (i = 1; i <= times; i++) {
        printf("%2d x %2d = %3d\n", i, base, base*i);
    }
        
    return 0;
}

