#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>

void sigint(int num) {
    printf("Count Ctrl-C\n\n");
    exit(-1);
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, sigint);

    char buffer[128];
    char inWord = 0;
    unsigned numwords = 0;
    while (1) {
        int numread = read(0, buffer, 128);
        if (numread == 0) break;
        int i;
        for (i=0; i<numread; i++) {
            if (isspace(buffer[i])) {
                if (inWord) {
                    numwords++;
                    inWord = 0;
                }
            } else {
                inWord = 1;
            }
        }
    }
    printf("\t%d\n", numwords);
    return 0;
}

