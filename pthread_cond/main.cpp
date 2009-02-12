#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>
#include <signal.h>

#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_NORMAL "\033[22;37m"

#define PRINT(a) printf("%s%s "a ANSI_NORMAL"\n", color, __func__)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  confirmed = PTHREAD_COND_INITIALIZER;
int done;

#define ETIMEDOUT 110

void* waiter(void *) {
    const char* color = ANSI_GREEN;
    PRINT("start");

    pthread_mutex_lock(&mutex);
    PRINT("waiting");
    done = 0;
    int retcode = 0;
#ifdef WITH_TIMEOUT
    while (!done) {
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec = timeout.tv_sec + 1;
        retcode = pthread_cond_timedwait(&confirmed, &mutex, &timeout);
        if (retcode == ETIMEDOUT) PRINT("timeout");
    }
#else
    while (!done) {
        retcode = pthread_cond_wait(&confirmed, &mutex);
        if (retcode != 0) printf("retcode = %d\n", retcode);
    }
#endif
    PRINT("waiting done");
    pthread_mutex_unlock(&mutex);

    PRINT("finish");
    pthread_exit(0);
}

void* unlocker(void *) {
    const char* color = ANSI_RED;
    PRINT("start");
    
    PRINT("sleeping 3 secons");
    sleep(3);
    PRINT("unlocking");
    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&confirmed);
    pthread_mutex_unlock(&mutex);
    PRINT("unlocking done");

    PRINT("finish");
    pthread_exit(0);
}


void sigtrap(int) {
    printf("Ctrl-C\n");
}


int main() {
    pthread_t threads[2];

    signal(SIGINT, sigtrap);

    pthread_create(&threads[0], 0, waiter, (void *)0);
    pthread_create(&threads[1], 0, unlocker, (void *)0);

    pthread_join(threads[0], 0);
    pthread_join(threads[1], 0);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&confirmed);

    return 0;
}

