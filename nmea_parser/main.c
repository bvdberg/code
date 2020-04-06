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

#include "color.h"

// TODO fix CR/LF for LINE_SIZE 2
#define LINE_SIZE 128

#define CR 0x0D
#define LF 0x0A

typedef struct {
    char line_buf[256];
    char* line_end;
    bool found_newline;
} nmea_parser;


static void nmea_parser_init(nmea_parser* nmea) {
    memset(nmea, 0, sizeof(nmea_parser));
    nmea->line_end = nmea->line_buf;
}

static void nmea_handle_line(const char* line) {
    printf("-> %s\n", line);
}

static void nmea_parser_add(nmea_parser* nmea, char* line) {
    printf("%s%s%s\n", ANSI_GREEN, line, ANSI_NORMAL);
    char* cp = line;
    if (!nmea->found_newline) {
        while (*cp) {
            if (cp[0] == CR && cp[1] == LF) {
                nmea->found_newline = true;
                cp += 2;
                break;
            }
            cp++;
        }
    }

    if (!nmea->found_newline) return;

    // copy lines into line_buf
    char* line_start = cp;
    char* op = nmea->line_end;
    while (*cp) {
        // TODO does not go right every time
        if (cp[0] == CR && cp[1] == LF) {
            *op = 0;
            nmea_handle_line(nmea->line_buf);
            op = nmea->line_buf;
            cp += 2;
            continue;
        }
        if (cp[0] != LF) {  // remove newlines
           *op = *cp;
           op++;
        }
        cp++;
    }
    *op = 0;
    nmea->line_end = op;
}

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

    nmea_parser nmea;
    nmea_parser_init(&nmea);

    while (1) {
        char line[LINE_SIZE];
        int numread = read(fd, line, sizeof(line)-1);
        if (numread == -1) {
            fprintf(stderr, "error reading %s: %s\n", filename, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (numread == 0) break;

        line[numread] = 0;
        nmea_parser_add(&nmea, line);
    }


    printf("done\n");
    close(fd);
    return 0;
}

