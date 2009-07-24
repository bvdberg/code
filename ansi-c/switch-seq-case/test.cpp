#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int num = atoi(argv[1]);

    switch(num) {
        case 1 ... 4:
            printf("1 ... 4\n");
            break;
        default:
            printf("default\n");
            break;
    }

    return 0;
}

