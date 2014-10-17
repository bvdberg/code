/*
    This tools does what shell does with: 'process1 2>&1 | process2'
    On Ctrl-C it terminates both (or first)

    use: ./test ./writer ./reader
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

static void writeChecked(int fd, const char* data) {
    ssize_t len = strlen(data);
    ssize_t numwritten = write(fd, data, len);
    if (numwritten != len) {
        // ignore
    }
}

static void error(const char* msg) {
    char buffer[256];
    if (errno == 0) {
        sprintf(buffer, "ERROR pid=%d %s\n", getpid(), msg);
    } else {
        sprintf(buffer, "ERROR pid=%d %s: %s\n", getpid(), msg, strerror(errno));
    }
    int fd = open("error.log", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) return;
    writeChecked(fd, buffer);
    close(fd);
    exit(-1);
}

static void child_signal(int n) {
    printf("SIGCHLD\n");
}

static void term_signal(int n) {
    printf("SIGTERM\n");
}

static pid_t start_app(const char* prog, int fd0, int fd1, int fd2) {
    printf("starting '%s'  in=%d  out=%d  err=%d\n", prog, fd0, fd1, fd2);
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(-1);
    }
    if (child_pid == 0) { //child

        close(0);
        int fd = dup(fd0);
        if (fd == -1) error("CHILD dup(fd0)\n");

        close(1);
        fd = dup(fd1);
        if (fd == -1) {
            error("CHILD dup(fd1)");
        }

        close(2);
        fd = dup(fd2);
        if (fd == -1) error("CHILD dup(fd2)");

        execl(prog, prog, NULL);
        error("MAIN: error starting child\n");
        exit(-1);
    } else {   //parent
        printf("MAIN: started [%s] - pid %d\n", prog, child_pid);
    }
    return child_pid;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s [prog1] [prog2]\n", argv[0]);
        exit(1);
    }
    const char* prog1 = argv[1];
    const char* prog2 = argv[2];

    // create pipe
    int redirect[2];
    if (pipe(redirect) != 0) {
        perror("pipe");
        exit(-1);
    }

    int input = dup(0);
    if (input == -1) {
        perror("dup(0)");
        exit(-1);
    }
    int output = dup(1);
    int error = dup(2);

    int devnull = open("/dev/null", O_RDWR);
    if (devnull == -1) {
        perror("open");
        exit(-1);
    }

    pid_t pid1=0, pid2 = 0;
    pid1 = start_app(prog1, input, redirect[1], redirect[1]);
    pid2 = start_app(prog2, redirect[0], output, error);

    close(input);
    close(output);
    close(error);
    close(devnull);
    close(redirect[0]);
    close(redirect[1]);

    signal(SIGCHLD, child_signal);
    signal(SIGTERM, term_signal);

    // TODO close redirect[0] + redirect[1]

    while (1) {
        printf("MAIN: waiting for children\n");
        int status = 0;
        pid_t child_pid = waitpid(-1, &status, 0);
        if (child_pid == -1) {
            perror("waitpid");
            exit(-1);
        }
        printf("MAIN: waitpid pid=%d\n", child_pid);

        if (child_pid == pid1) {
            printf("prog1 stopped\n");
        }
        if (child_pid == pid2) {
            printf("prog2 stopped\n");
        }
    }

    // TODO return code of prog1 (if normal termination), -1 if crashed
    return 0;
}

