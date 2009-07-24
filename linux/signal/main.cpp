#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

jmp_buf buf;
int val;
bool stopped;

void sigint(int num) {
    printf("caught Ctrl-C (%d)\n", num);
    longjmp(buf, val);
}


void infiniteLoop() {
    printf("doing infinite loop\n");
    while(1) {
        printf("sleeping\n");
        sleep(2); 
    }
}


int main(int argc, char* argv[]) {
    stopped = false;
    signal(SIGINT, sigint);

    if (true) {
        infiniteLoop();
    } else {
        printf("not doing loop\n");
    }
    return 0;
}

