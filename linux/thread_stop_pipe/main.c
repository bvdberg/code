#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>

// HOWTO: uncomment none/1 scenario at a time to see results
//#define SCENARIO1
//#define SCENARIO2
//#define SCENARIO3
//#define SCENARIO4

/*
    Observed behaviour on Ubuntu 14.04:

    NORMAL (no scenarios)
        thread: start
        main: writing stop
        thread: select = 1
        thread: read = 1
        thread: got cmd 'q'
        thread: end

    SCENARIO1: close(pipefd[1]) before stop()
        thread: start
        thread: select = 1
        thread: read = 0
        thread: pipe closed
        thread: end

    SCENARIO2: close(pipefd[0]) after select() (from main thread)
        thread: start
        main: writing stop
        (thread blocks forever on select())


    SCENARIO3: do write(pipefd[1] before select()
        main: writing stop
        thread: start
        thread: select = 1
        thread: read = 1
        thread: got cmd 'q'
        thread: end
        main: stopping thread
        main: writing stop

    SCENARIO4: close(pipefd[1]) before select()
        thread: start
        thread: select = -1
        select: Bad file descriptor
*/

static pthread_t thread;
static int pipefd[2];

static void writeStop() {
    printf("main: writing stop\n");
    char cmd = 'q';
    if (write(pipefd[1], &cmd, 1) != 1) {
        perror("write");
        exit(-1);
    }
}

static void stop() {
#ifndef SCENARIO1
    writeStop();
#endif
    if (pthread_join(thread, 0) != 0) {
        perror("join");
        exit(-1);
    }
    close(pipefd[0]);
    close(pipefd[1]);
}

static void* thread_main(void* arg) {
    printf("thread: start\n");

    int fd = pipefd[0];

    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        int result = select(fd + 1,  &rfds, NULL, NULL, NULL);
        printf("thread: select = %d\n", result);

        if (result == -1) {
            perror("select");
            exit(-1);
        }
        if (result == 0) continue;

        if (FD_ISSET(fd, &rfds)) {
            char buffer[4];
            int numread = read(fd, buffer, 1);
            printf("thread: read = %d\n", numread);
            if (numread < 0) {
                perror("read");
                exit(-1);
            }
            if (numread == 0) {
                printf("thread: pipe closed\n");
                break;
            }

            printf("thread: got cmd '%c'\n", buffer[0]);
            break;
        }
    }

    printf("thread: end\n");
    return 0;
}

int main(int argc, const char *argv[])
{
    if (pipe(pipefd) != 0) {
        perror("creating pipe");
        exit(-1);
    }

#ifdef SCENARIO3
    writeStop();
#endif
#ifdef SCENARIO4
    close(pipefd[0]);
#endif

    if (pthread_create(&thread, 0, thread_main, NULL) != 0) {
        perror("creating thread");
        exit(-1);
    }

    usleep(250000);

#ifdef SCENARIO1
    close(pipefd[1]);
#endif
#ifdef SCENARIO2
    close(pipefd[0]);
#endif
    stop();

    return 0;
}

