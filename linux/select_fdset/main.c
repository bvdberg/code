#include <stdio.h>
#include <sys/select.h>

// prints the hexmap of fd_set.
// fd x is set op position x, STARTING FROM ZERO!!
// so fd 1 is set[1]

void printSet(fd_set *set) {
    int i;
    for (i=0; i<8; i++) {
        long int* p = (long int*)set;

        printf("%08x  ", p[i]);
    }
    printf("\n");
}

int main(int argc, const char *argv[])
{
    int i;
    for (i=1; i<128; i++) {
        printf("%3d -> ", i);
        fd_set rdfs;
        FD_ZERO(&rdfs);   
        FD_SET(i, &rdfs);

        printSet(&rdfs);
    }
    return 0;
}

