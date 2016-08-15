#include "Derived.h"
#include <stdio.h>

Derived::Derived(int id_) : Base(id_) {
    printf("Derived()\n");
}

Derived::~Derived() {
    printf("~Derived()\n");
}

