#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>


void sigtrap(int) {
    printf("Ctrl-C caught, press Ctrl-\\ to quit\n");
}


int main() {
    signal(SIGINT, sigtrap);

    while (1) {
        usleep(100000);
    }

    return 0;
}

