#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void printChar(const char* text, const char* cp) {
    printf("%s: char = %d\n", text, *cp);
}

int main(int argc, const char *argv[])
{
    char* c = (char*)malloc(1000);
    memset(c, 0, 1000);
    printf("Hello\n");

    char* cp = c;
    printChar("before", cp);
    free(c);
    printChar("after ", cp);
    return 0;
}

