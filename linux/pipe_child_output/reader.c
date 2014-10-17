#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
    time_t now = time(0);
    fprintf(stdout, "reader: test output %ld\n", now);
    fprintf(stderr, "reader: test error %ld\n", now);

    while (1) {
        char buffer[128];
        int numread = read(0, buffer, sizeof(buffer)-1);
        if (numread == -1) {
            fprintf(stderr, "reader: error on read(): %s\n", strerror(errno));
            exit(-1);
        }
        if (numread == 0) {
            fprintf(stderr, "reader: end of stream\n");
            break;
        }
        buffer[numread] = 0;
        buffer[numread-1] = 0;  // strip newline
        printf("reader: read '%s'\n", buffer);

    }

    printf("reader: end\n");
    return 0;
}

