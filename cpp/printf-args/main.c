#include <stdio.h>

int main() {
    // the arguments can be switched
    printf("%1$s %2$s\n", "first", "second");
    printf("%2$s %1$s\n", "first", "second");

    printf("%2$d %1$d\n", 1, 2);

    return 0;
}

