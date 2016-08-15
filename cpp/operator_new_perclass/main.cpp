#include <stdio.h>
#include "Base.h"
#include "Derived.h"
#include "Context.h"

int main(int argc, const char *argv[])
{
    Context C;

    printf("main: creating Base\n");
    //Base* b = new Base(1);     <- not allowed
    Base* b = new (C) Base(1);
    // TODO fix delete
    //delete b;

    printf("main: creating Derived\n");
    Derived* d = new (C) Derived(2);
    //delete d ;
    return 0;
}

