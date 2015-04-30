#include <stdio.h>

#include "Bar.h"
#include "Foo.h"

int main(int argc, const char *argv[])
{
    Foo f1(10);
    Bar b1;
    b1.add(f1);
    return 0;
}

