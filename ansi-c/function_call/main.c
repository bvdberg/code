#include <stdio.h>

int function(int* len)
{
     printf("len=%d\n", *len);
     return *len;
}


int main(int argc, const char *argv[])
{
    int len = 10;
    int total = 123;
    int ok = 33;
    ok = function(&len);
    total += ok;
    ok = function(&len);
    total += ok;
    ok = function(&len);
    total += ok;
    ok = 44;
    printf("total=%d\n", total);
    return 0;
}

