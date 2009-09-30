#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    printf("cat /proc/%d/status\n", getpid());
    sleep(6);

    printf("malloc 1 Mb\n");
    void* mem1 = malloc(1024*1024);
    sleep(6);

    printf("malloc 1 Mb\n");
    void* mem2 = malloc(1024*1024);
    sleep(6);

    printf("freeing 1 Mb\n");
    free(mem2);
    sleep(6);
    
    printf("freeing 1 Mb\n");
    free(mem1);
    sleep(6);

    return 0;
}

