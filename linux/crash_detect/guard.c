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


typedef struct {
    pid_t pid;
    char executable[64];
} Child;

#define MAX_ARGS 20

static Child start_app(const char* prog, char* argv[]) {
    Child child;
    strncpy(child.executable, prog, 63);
    pid_t child_pid = fork();
    if (child_pid == 0) { //child
        usleep(50000);
        //execvp(argv[1], NULL);
        //execlp(prog, prog, (char*)NULL);
        execv(prog, argv);
        printf("guard: error starting %s\n", prog);
        child.pid = 0;
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
    Child child = start_app(argv[1], &argv[1]);

    int count = 0;
    while (count < 4) {
        //phase 2 - wait until first one terminates
        int status = 0;
        pid_t child_pid = waitpid(child.pid, &status, 0);

        count++;
        if (!WIFEXITED(status)) { //child exits abnormally
            if (child.pid == child_pid) {
                printf("guard: process [%s] (pid %d) crashed!\n", child.executable, child.pid);
                child = start_app(argv[1], &argv[1]);
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

