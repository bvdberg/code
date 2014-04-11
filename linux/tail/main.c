#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

/*
    Usage: bbr_tail -n [lines] -s [stepsize] [inputfile]
        lines: output %d lines
        stepsize 3 means: take 1 out of 3 lines

    This program does:
    - ignore lines starting with '#'
    - ignore lines not ending with newline/CR
    - output lines/step
*/

#define LF 0x0A
#define MAX_LINES 10000

typedef struct {
    const char* start;
    int size;
} Line;

static char* findStart(const char* begin, char* cp) {
    if (cp == begin) return cp;
    cp--;
    while (cp != begin) {
        if (*cp == LF) return cp + 1;
        cp--;
    }
    return cp;
}

int main(int argc, const char *argv[])
{
    // check arguments
    if (argc != 6) {
        printf("Usage %s -n [lines] -s [stepsize] [inputfile]\n", argv[0]);
        return -1;
    }
    const char* numlines_str = argv[2];
    const char* stepsize_str = argv[4];
    const char* infile = argv[5];

    int numlines = atoi(numlines_str);
    int stepsize = atoi(stepsize_str);
    if (numlines > MAX_LINES) {
        printf("Max lines = %d\n", MAX_LINES);
        return -1;
    }

    // check arguments, open files etc
    int fd = open(infile, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) != 0) {
        perror("fstat");
        return -1;
    }

    int size = statbuf.st_size;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    void* input_map = mmap(NULL, size, prot, flags, fd, 0);
    if (input_map == (void*)-1) {
        perror("mmap");
        return -1;
    }
    close(fd);

    // create output buffer
    Line* output = (Line*)calloc(numlines, sizeof(Line));
    if (output == 0) return -2;

    // read file
    const char* begin = (const char*) input_map;
    const char* end = begin + size -1;  // last char
    char* cp = (char*)end;

    int numOutputLines = 0;
    int stepsToSkip = 0;
    while (numOutputLines < numlines) {
        // read a line, cp should point to last char on line
        char* line = findStart(begin, cp);
        if (*cp != LF || *line == '#') {
            // skip line
        } else {
            // for steps, always print first line, then skip step-1 lines
            if (stepsToSkip == 0) {
                Line* cur = &output[numOutputLines];
                cur->start = line;
                cur->size = cp + 1 - line;
                numOutputLines++;
                stepsToSkip = stepsize;
            }
            stepsToSkip--;
        }
        if (line == begin) break;   // start of file reached
        cp = line - 1;
    }

    // print output
    int i;
    for (i=numOutputLines-1; i >= 0; i--) {
        Line* cur = &output[i];
        int written = write(1, cur->start, cur->size);
        if (written != cur->size) {
            perror("write");
        }
    }

    // cleanup
    munmap(input_map, size);
    free(output);
    return 0;
}

