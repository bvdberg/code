#include <iostream>
#include <stdio.h>

class Foo {
public:
    Foo()
        : n(1)
    {
        printf("Foo()\n");
    }

    Foo(int a)
        : Foo()
        // , n(a)  -> NOT allowed, only 1
    {
        n = a;
        printf("Foo(%d)\n", a);
    }

    int n;
};

int main(int argc, const char *argv[])
{
    Foo f(10);
    printf("n = %d\n", f.n);

    return 0;
}

