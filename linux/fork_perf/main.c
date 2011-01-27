#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

u_int64_t getNow()
{
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_usec);
    return now64;
}

int main(int argc, const char *argv[])
{
    int runs = 10000;
    int current = 0;

    u_int64_t t1 = getNow();
    while (current < runs) {
        pid_t pid = fork();
        switch (pid) {
        case -1:
            perror("fork");
            exit(-1);
        case 0:     // child
            // TODO exec ?
            exit(0);
        default:
            break;
        }
        current++;
    }
    u_int64_t t2 = getNow();
    printf("%d forks in %llu usec, %llu usec/fork\n", runs, t2-t1, (t2-t1)/runs);
    return 0;
}

