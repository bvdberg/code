#include <stdio.h>
#include <math.h>

// Carmack's Unusual Inverse Square Root
float InverseSquareRoot( float number ) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;  // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

    return y;
}

float SquareRootFloat(float number) {
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}

void mysqrt(float f) {
    printf("%f -> %f  (math %f)\n", f, SquareRootFloat(f), sqrt(f));
}

int main() {
    mysqrt(2);
    mysqrt(4);
    mysqrt(16);
    mysqrt(100);
    mysqrt(600);
    mysqrt(1000);
    return 0;
}

