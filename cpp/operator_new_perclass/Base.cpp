#include "Base.h"
#include <stdio.h>

#include "Context.h"

Base::Base(int id_) : id(id_) {
    printf("Base() id=%d\n", id);
}

Base::~Base() {
    printf("~Base()\n");
}

void* Base::operator new(size_t bytes, const Context& C) {
    printf("Base::new %ld\n", bytes);
    // TODO alignment
    return ::operator new(bytes, C, 8);
}

