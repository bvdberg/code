#include <stdio.h>

int main(int argc, const char *argv[])
{
    printf("'");
    int i;
    for (i=1; i<argc; i++) {
        int val = atoi(argv[i]);
        printf("%c", val);
    }
    printf("'\n");
    return 0;
}

