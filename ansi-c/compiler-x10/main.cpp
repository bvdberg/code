#include <stdio.h>
#include <stdlib.h>

int times10(int a) {
    return a*10;
}

int times100(int a) {
    return a*100;
}

int times64(int a) {
    return a*64;
}

int main(int argc, char* argv[]) {
    int b = times10(atoi(argv[1]));
    printf("result = %d\n", b);
    return 0;
}

