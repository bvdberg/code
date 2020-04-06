#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include "nmea_parser.h"

#define LINE_SIZE 128
//#define LINE_SIZE 2

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char* filename = argv[1];
    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1) {
        fprintf(stderr, "error opening %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    nmea_parser* parser = nmea_parser_create();

    while (1) {
        char line[LINE_SIZE];
        int numread = read(fd, line, sizeof(line)-1);
        if (numread == -1) {
            fprintf(stderr, "error reading %s: %s\n", filename, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (numread == 0) break;

        line[numread] = 0;
        nmea_parser_add(parser, line);
    }


    nmea_parser_destroy(parser);
    printf("done\n");
    close(fd);
    return 0;
}

