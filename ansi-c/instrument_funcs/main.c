#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

/*
 *  strip whitespace from file:
 *  + print lines
 *  + support both newline chars
 *  + replace newlines with '\n'
 *  + empty lines
 *  - wildcards: *.c
 */


#define LF 10
#define CR 13
#define TAB 0x09
#define SPACE 0x20

#define ANSI_BRED "\033[01;31m"
#define ANSI_NORMAL "\033[0m"
#define ANSI_BGRED "\33[0;37;40m"
#define ANSI_BGGREY "\33[0;37;40m"


static void checkLine(int lineNr, const char* line, const char* end, int output)
{
    int size = end - line;
    if (size == 0) {
        if (output) write(output, "\n", 1);
        return;
    }
    assert(size < 512);
    char buffer[512];
    memcpy(buffer, line, size);
    buffer[size] = 0;

    char* cp = buffer+size-1;
    // strip off LF/CR
    while (cp != buffer) {
      if (*cp != CR && *cp != LF) break;
      cp--;
    }
    // strip off whitespace
    while (cp != buffer) {
        if (*cp != TAB && *cp != SPACE) break;
        cp--;
    }
    if (*cp == TAB || *cp == SPACE) {
        // whitespace line
        if (output) write(output, "\n", 1);
        return;
    }
    *(cp+1) = '\n';
    *(cp+2) = 0;
    if (output) write(output, buffer, strlen(buffer));
}

 
static void parseFile(const char* file, unsigned int size, int output)
{
    int lineNr = 1;
    const char* line = file;
    const char* cp = line;
    while (cp - file < size) {
        if (*cp == LF || *cp == CR) {
            checkLine(lineNr, line, cp, output);
            lineNr++;
            if ((cp - file) + 1 <= size) {
                if (*cp == CR && *(cp+1) == LF) cp++;
                else if (*cp == LF && *(cp+1) == CR) cp++;
            }
            line = cp+1;
        }
        cp++;
    }
    if (line != file+size) checkLine(lineNr, line, cp, output);
}


int main(int argc, const char *argv[])
{
    const char* filename = "file.txt";
    const char* output_file = "out.txt";

    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("open"); return 0; }

    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        close(fd);
        return 0;
    }
    unsigned int size = statbuf.st_size;

    void* map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); return 0; }
    close(fd);
    
    int output = open(output_file, O_WRONLY | O_CREAT, 0644);
    if (output == -1) {
        perror("open");
        goto out;
    }
    parseFile(map, size, output);
out:
    munmap(map, size);

    return 0;
}

