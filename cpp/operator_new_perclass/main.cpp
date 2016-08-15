#include <stdio.h>
#include "Base.h"
#include "Derived.h"
#include "Foo.h"
#include "Context.h"

Buffer<T, 4> typeExprBuffer;

static uint8_t buffer[4 * sizeof(Foo)];
static uint8_t fooTaken[4];

void* getFoo() {
    unsigned i;
    for (i=0; i<4; i++) {
        if (fooTaken[i] == 0) break;
    }
    if (i == 4) {
        assert(0 && "not enough Foo's");
        return 0;
    }
    fooTaken[i] = 1;
    printf("FOO ALLOC %d - %p\n", i, &buffer[i * sizeof(Foo)]);
    return &buffer[i * sizeof(Foo)];
}

void freeFoo(Foo* f) {
    uint8_t* cp = (uint8_t*)f;
    unsigned index = (cp - buffer) / sizeof(Foo);
    fooTaken[index] = 0;
    printf("FOO FREE %p -> %d\n", f, index);
}


int main(int argc, const char *argv[])
{
    Context C;

    printf("main: creating Base\n");
    //Base* b = new Base(1);     // <- not allowed
    Base* b = new (C) Base(1);
    //delete b;     // <- not allowed

    printf("main: creating Derived\n");
    Derived* d = new (C) Derived(2);
    for (unsigned i=0; i<1024; i++) {
        new (C) Derived(i);
    }

#if 1
    // placement operator new
    Foo* f1 = new (getFoo()) Foo(1);
    printf("f1 = %p  id=%d\n", f1, f1->getID());
    Foo* f2 = new (getFoo()) Foo(2);
    printf("f2 = %p  id=%d\n", f2, f2->getID());
    Foo* f3 = new (getFoo()) Foo(3);
    printf("f3 = %p  id=%d\n", f3, f3->getID());
    Foo* f4 = new (getFoo()) Foo(4);
    printf("f4 = %p  id=%d\n", f4, f4->getID());

    freeFoo(f2);
    Foo* f5 = new (getFoo()) Foo(5);
    printf("f5 = %p  id=%d\n", f5, f5->getID());
#endif

    C.dump();
    return 0;
}

