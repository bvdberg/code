/*
    - Start a cgroup for subprocess
    - start child in cgroup
    - kill all processes in cgroup (test with daemonized children)
    - cleanup cgroup
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


#include "CGroupUtil.h"

using namespace Luna;

typedef struct {
    pid_t pid;
    char executable[64];
} Child;

#define MAX_ARGS 20

static void writeChecked(int fd, const char* data) {
    ssize_t len = strlen(data);
    ssize_t numwritten = write(fd, data, len);
    if (numwritten != len) {

    }
}

static void error(const char* msg) {
    int fd = open("error.log", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) return;
    writeChecked(fd, msg);
    close(fd);
    exit(-1);
}

static Child start_app(const char* prog, char* argv[], int redirect) {
    Child child;
    strncpy(child.executable, prog, 63);
    pid_t child_pid = fork();
    if (child_pid == 0) { //child
        usleep(50000);
        execv(prog, argv);
        child.pid = 0;
        fprintf(stderr, "---- [%ld] failed to start: %s: %s\n", time(0), prog, strerror(errno));
        fflush(stderr);
        error("GUARD: error starting child\n");
    } else {   //parent
        printf("guard: started [%s] - pid %d\n", prog, child_pid);
        child.pid = child_pid;
    }
    return child;
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s [prog] <arg1> <arg2>\n", argv[0]);
        exit(1);
    }

    //phase 1 - start child process
    Child child = start_app(argv[1], &argv[1], 1);

    int status = 0;
    pid_t child_pid = waitpid(child.pid, &status, 0);

    if (!WIFEXITED(status)) { //child exits abnormally
        if (child.pid == child_pid) {
            printf("guard: process [%s] (pid %d) crashed!\n", child.executable, child.pid);
            child = start_app(argv[1], &argv[1], 1);
        }
        // else weird
    } else {
        if (child.pid == child_pid) {
            printf("guard: process [%s] (pid %d) stopped normally, returned %d\n", child.executable, child.pid, (char)WEXITSTATUS(status));
            child.pid = 0;
        }
        // else weird
    }

    return 0;
}

