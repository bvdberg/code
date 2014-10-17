/*
    helper tool to make sure a program is running at all times.
    it starts the executable indicated by the first cmdline parameter
    and checks if it exited normally. If the program didn't, it will
    be restarted.
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

typedef struct {
    pid_t pid;
    char executable[64];
} Child;

#define MAX_ARGS 20

static void writeChecked(int fd, const char* data) {
    size_t len = strlen(data);
    int numwritten = write(fd, data, len);
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
        int fdout = -1;
        int fderr = -1;
        if (redirect) {
            fflush(stdout);
            close(1);
            fdout = open("./out.log", O_APPEND | O_CREAT | O_WRONLY, 0644);
            if (fdout == -1) {
                error("open(out)");
            }

#if 1
            // same file
            close(2);
            fderr = dup(1);
            if (fderr  == -1) {
                error("dup() returned error");
            }
#else
            // different file
            fflush(stderr);
            close(2);
            fderr = open("./err.log", O_APPEND | O_CREAT | O_WRONLY, 0644);
            if (fderr == -1) {
                error("open(err)");
            }
#endif
            fprintf(stderr, "---- [%ld] starting %s ----\n", time(0), prog);
            fflush(stderr);
            //writeChecked(1, "--- NEW OUTPUT ---\n");
            //writeChecked(2, "--- NEW ERROR ---\n");
            // NOTE printf's on stdout are sometimes not seen in resulting file (exec does not flusH)
            //syncfs(1);
            //syncfs(2);
        }
        //execvp(argv[1], NULL);
        //execlp(prog, prog, (char*)NULL);
        execv(prog, argv);
        child.pid = 0;
        fprintf(stderr, "---- [%ld] failed to start: %s: %s\n", time(0), prog, strerror(errno));
        fflush(stderr);
        close(fdout);
        close(fderr);
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

    int count = 0;
    while (count < 4) {
        //phase 2 - wait until first one terminates
        int status = 0;
        pid_t child_pid = waitpid(child.pid, &status, 0);

        count++;
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
            break;
        }
    }

    return 0;
}

