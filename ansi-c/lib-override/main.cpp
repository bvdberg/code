#include "lib1.h"

/* 
    if you run : LD_PRELOAD=liboverride.so ./test
    the override.cpp will be used
*/


int main() {
    lib1_print();
    return 0;
}

