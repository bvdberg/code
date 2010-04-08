#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
  
/*
 *  strip whitespace from file:
 *  - empty lines
 *  - lines ending with spaces + newline
 *  - support both newline chars
 *  - print lines
 */


#define LF 10
#define CR 13
#define TAB 0x09
#define SPACE 0x20

#define ANSI_BRED "\033[01;31m"
#define ANSI_NORMAL "\033[0m"
#define ANSI_BGRED "\33[0;37;40m"
#define ANSI_BGGREY "\33[0;37;40m"

static void checkLine(int lineNr, char* line, char* end)
{
    int size = end - line;
    if (size == 0) {
        //printf("empty line: [%d]\n", lineNr);
        return;
    }
    char* cp = end-1;   
    while (cp != line) {  
        if (*cp != TAB && *cp != SPACE) break;
        cp--;
    }
    int white = (end-1) - cp;
    if (white) {
        char normal[256];
        char space[256];
        memset(normal, 0, sizeof(normal));
        memset(space, 0, sizeof(space));
        if (*cp == TAB || *cp == SPACE) {
            memcpy(space, line, size-1);
        } else {
            memcpy(normal, line, cp+1-line); 
            memcpy(space, cp+1, (end-cp)-1);
        }
//        printf("[%d] normal=[%s]  white=[%s]\n", lineNr, normal, space);
        printf("[%d] "ANSI_BGGREY"%s"ANSI_BGRED"%s"ANSI_NORMAL"\n", lineNr, normal, space);
    }
}

 
static void parseFile(char* file, unsigned int size)
{
    int lineNr = 1;
    char* line = file;
    char* cp = line;
    while (cp - file < size) {
        if (*cp == LF || *cp == CR) {
            checkLine(lineNr, line, cp);
            lineNr++;
            line = cp+1;
            // TODO read next enters
        }
        cp++;
    }
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage %s [file]\n", argv[0]);
        return 0;
    }
    const char* filename = argv[1];

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

    parseFile(map, size);

    munmap(map, size);

    return 0;
}

