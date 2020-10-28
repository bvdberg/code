#include <stdio.h>

typedef struct {
    int a;
    int b;
    char c;
    char d[100];
} foo;

char array[sizeof(foo) < 100 ? 1 : -1];

int main(int argc, const char *argv[])
{
    
    return 0;
}

