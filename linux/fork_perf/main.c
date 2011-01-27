#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#ifdef USE_PTHREAD
void* thread_main(void* arg) {
    pthread_exit(0);
}
#endif

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

#ifdef USE_FORK
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
            {
                // wait for last child
                int status = 0;
                waitpid(pid, &status, 0);
            }
            break;
        }
        current++;
    }
    u_int64_t t2 = getNow();
    printf("%d forks in %llu usec, %llu usec/fork\n", runs, t2-t1, (t2-t1)/runs);
#endif

#ifdef USE_PTHREAD
    u_int64_t t1 = getNow();
    while (current < runs) {
        pthread_t thread;
        pthread_create(&thread, 0, thread_main, 0);
        pthread_join(thread, 0);
        current++;
    }
    u_int64_t t2 = getNow();
    printf("%d pthreads in %llu usec, %llu usec/fork\n", runs, t2-t1, (t2-t1)/runs);
#endif
    return 0;
}

