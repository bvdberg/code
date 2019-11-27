#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "events.h"
#include "logger.h"

#define UNUSED(x) (void)(x)
//#define BUFSIZE 4096
#define BUFSIZE 1024*32
/*
    Read-Write copy:
    Bufsize     MB/Sec          Splice
      4K        1900
      8K        2500
     16K        3100            3700
     32K        3400            5000
     64K        3800            3700
    128K        1500 <- weird
*/

static uint64_t total_read;
static uint64_t total_size;

static void on_read(int fd, void* arg, short flags) {
    UNUSED(flags);
    event_base_t* base = (event_base_t*)arg;
    char buffer[BUFSIZE];
    ssize_t numread = read(fd, buffer, BUFSIZE);
    if (numread == -1) {
        log_fatal("child: error reading %s", strerror(errno));
    }
    total_read += numread;
    //printf("child: read %lu/%lu\n", total_read, total_size);
    if (total_read >= total_size || numread == 0) {
        events_loopbreak(base, EXIT_FAILURE);
    }
}

int main(int argc, const char *argv[])
{
    const char* filename = "data";
    if (argc == 2) filename = argv[1];
    log_init(Info, true, true);
    event_base_t* base = events_new();

    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "error opening pipe: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int fd2 = open(filename, O_RDONLY | O_CLOEXEC);
    if (fd2 == -1) {
        fprintf(stderr, "Error opening %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    struct stat statbuf;
    if (fstat(fd2, &statbuf) == -1) {
        fprintf(stderr, "cannot stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    uint64_t size = statbuf.st_size;
    total_size = size;

    log_info("size %llu, bufsize %u", size, BUFSIZE);
    event_t event;

    int parent = 0;
    if (fork() == 0) {  // child
        close(fd[1]);   // close write-end
        close(fd2);
        events_assign(&event, base, fd[0], EV_READ, on_read, base);
        events_add(&event);
        log_info("child: start");
        uint64_t t1 = current_time();
        uint8_t ret = events_mainloop(base);
        uint64_t t2 = current_time();
        log_info("child: done (%llu usec)", t2 - t1);
        log_info("child: %u MB/sec", total_size / (t2 - t1));
    } else { // parent
        parent = 1;
        close(fd[0]);   // close read-end

        uint64_t t1 = current_time();
        log_info("parent: start");
#if 0
        // use read-write
        char buffer[BUFSIZE];
        while (1) {
            ssize_t numread = read(fd2, buffer, BUFSIZE);
            if (numread == -1) {
                log_fatal("error reading file: %s", strerror(errno));
            }
            int written = write(fd[1], buffer, numread);
            if (written != numread) {
                log_fatal("error writing: %s", strerror(errno));
            }
            //printf("read %ld\n", numread);
            if (numread != BUFSIZE) break;
        }
#else
        // use splice
        uint64_t total_written = 0;
        while (1) {
            ssize_t res = splice(fd2, 0, fd[1], 0, total_size, 0);
            total_written += res;
            //log_info("parent: splice %lu / %lu", total_written, total_size);
            if (total_written == total_size) break;
        }
#endif
        uint64_t t2 = current_time();
        log_info("parent: done (%llu usec)", t2 - t1);
        close(fd2);
        sleep(1);   // wait for child
    }


    events_free(base);

    return 0;
}

