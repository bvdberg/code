// compile with: gcc test.c
#include <stdio.h>

void(*function)() = 0;

void printA() { printf("%s\n", __PRETTY_FUNCTION__); }
void printB() { printf("%s\n", __PRETTY_FUNCTION__); }

int main() {
    function = printA;
    if (function) function();
    
    if (function == printA) printf("equal\n");
    if (function == &printA) printf("equal\n");
    
    function = printB;
    if (function) function();

    return 0;
}

