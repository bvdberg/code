//
//  Copyright (c) 2005 Nederlandse Philips Bedrijven B.V.
//  Philips Research Laboratories
//  Eindhoven, The Netherlands
//
#include <stdio.h>
#include "ClassA.h"

ClassA::ClassA(int a_)
    : a(a_)
{
}


ClassA::~ClassA()
{
}


ClassA::operator int() const
{
    printf("operator int\n");
    return 10;
}


ClassA& ClassA::operator=(char* s)
{
    printf("operator =\n");
    return this;
}
