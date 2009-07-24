#include "ClassA.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("1:\n");
    ClassA a1(100);
    
    printf("2:\n");
    ClassA a2 = ClassA(200);
    
    printf("3:\n");
    ClassA a3 = 300;
    
    printf("4:\n");
    ClassA a4;
    a4 = 400;
    printf("done\n");
}

