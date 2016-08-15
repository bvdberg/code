#include "Foo.h"
#include <stdio.h>

Foo::Foo(int id_) : Base(id_) {
    //printf("Foo() id=%d\n", id_);
}

Foo::~Foo() {
    //printf("~Foo()\n");
}

