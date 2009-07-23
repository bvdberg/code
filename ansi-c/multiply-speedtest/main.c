/*
 * Test which is faster:  / 400 of * 0.0025
 * Conclusion: *0.0025 is much faster
 */

#include <stdio.h>
#include <stdarg.h>

int doThing() {
    return 10;
}

int main(int argc, char*argv) {
    doThing();
    int a =  0;
    int c = 400;
    while(c-- > 0) {
            a += c;
    }
    printf("a = %d\n", a);
    return 0;
    
}

