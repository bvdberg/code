#include "Bar.h"
#include "Foo.h"
#include <stdio.h>

void Bar::add(Foo f) {
    print(f);
}

void Bar::print(Foo f) {
    printf("Bar: ");
    f.print();
}

