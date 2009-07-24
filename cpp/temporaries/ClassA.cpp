
#include <stdio.h>
#include "ClassA.h"

ClassA::ClassA(int a, int b)
    : m(a)
    , n(b)
{
    printf("classA constructor (a=%d, b=%d)\n", a, b);
}


ClassA& ClassA::operator=(int a)
{
    printf("ClassA::operator=(int)\n");
    m=a;
    n=1;
    return *this;
}

