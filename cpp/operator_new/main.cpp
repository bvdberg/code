#include <stdio.h>
#include <stdlib.h>

#include "ClassA.h"
#include "ClassB.h"

using namespace Test;

inline void * operator new(size_t size) {
    printf("%s() size=%d\n", __func__, size);
    void *ptr = (void *)malloc(size);
    return (ptr);
};

inline void operator delete(void *p) {
    printf("%s()\n", __func__);
    free(p);
};


int main(int argc, const char *argv[])
{
    ClassA* a = new ClassA();     
    delete a;
    return 0;
}


