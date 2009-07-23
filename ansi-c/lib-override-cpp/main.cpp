#include "normal.h"

/* 
    if you run : LD_PRELOAD=liboverride.so ./test
    the override.cpp will be used
*/


int main() {
    Derived d;
    d.myprint();
    d.func2();
    return 0;
}

