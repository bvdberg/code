#include "ClassA.h"
#include <stdio.h>

int main(int argc, char* argv[])
{

    ClassA a(20);
    int b = a;
    printf("int = %d\n", b);

    char* s = "string";
    a = s;
    return 0;
}
