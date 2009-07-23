//
//  Copyright (c) 2005 Nederlandse Philips Bedrijven B.V.
//  Philips Research Laboratories
//  Eindhoven, The Netherlands
//
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
    return 10;
}

