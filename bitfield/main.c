
#include <stdio.h>

struct Bits {
    unsigned a:1;
    unsigned b:3;
    unsigned c:2;
    unsigned d:2;
};

int main() {

    Bits b;

    if (Bits.a == 0) printf("Bits.a = 0\n");

    return 0;
}

