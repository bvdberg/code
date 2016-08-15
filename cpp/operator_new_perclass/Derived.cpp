#include "Derived.h"
#include <stdio.h>

Derived::Derived(int id_) : Base(id_) {
    //printf("Derived() id=%d\n", id_);
}

Derived::~Derived() {
    //printf("~Derived()\n");
}

