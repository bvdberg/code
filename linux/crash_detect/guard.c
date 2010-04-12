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


struct child_t {
    pid_t pid;
    char executable[64];
};


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: watcher [prog]\n");
        exit(1);
    }

    //phase 1 - start child process
    struct child_t child;
    pid_t child_pid = fork();
    if (child_pid == 0) { //child
        usleep(50000);
        int res = execvp(argv[1], NULL);
        if (res == -1) {
            printf("ERROR executing %s\n", argv[1]);
            return -1;
        }
    } else {   //parent
        printf("Started [%s] - pid %d\n", argv[1], child_pid);
        child.pid = child_pid;
        strncpy(child.executable, argv[1], 63);
    }

    int count = 0;
    while (count < 4) {
        //phase 2 - wait until first one terminates
        int status = 0;
        child_pid = waitpid(child_pid, &status, 0);
        printf("STATUS = %d\n", status);

        if ( WIFEXITED(status) == 0 ) { //child exits abnormally
            if (child.pid == child_pid) {
                printf("Watcher: Process [%s] (pid %d) crashed!\n", child.executable, child.pid);
                count++;
                child.pid = 0;
                //restart after crash
                child_pid = fork();
                if (child_pid == 0) { //child
                    int res = execvp(child.executable, NULL);
                    if (res == -1) {
                        printf("ERROR executing %s\n", child.executable);
                        return -1;
                    }
                } else {   //parent
                    printf("Restarted [%s] - pid %d\n", child.executable, child_pid);
                    child.pid = child_pid;
                }
            }
            // else weird
        } else {
            if (child.pid == child_pid) {
                printf("Process [%s] (pid %d) stopped normally\n", child.executable, child.pid);
                child.pid = 0;
            }
            // else weird
        }
    }

    return 0;
}

