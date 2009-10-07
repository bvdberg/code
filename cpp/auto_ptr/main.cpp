#include <stdio.h>

#include "ClassA.h"
#include "ClassB.h"

int main(int argc, char* argv[])
{
    printf("create:\n");
    ClassA* a = new ClassA();

    a->init();

    printf("destruct:\n");
    delete a;

    return 0;
}

