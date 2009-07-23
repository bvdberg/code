#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>
#include <signal.h>

#define ANSI_BLACK "\033[22;30m"
#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_BROWN "\033[22;33m"
#define ANSI_BLUE "\033[22;34m"
#define ANSI_MAGENTA "\033[22;35m"
#define ANSI_CYAN "\033[22;36m"
#define ANSI_GREY "\033[22;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED "\033[01;31m"
#define ANSI_BGREEN "\033[01;32m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_BBLUE "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_NORMAL "\033[22;37m"

#define PRINT(a) printf("%s%s "a ANSI_NORMAL"\n", color, __func__)

pthread_mutex_t mutex;
pthread_cond_t  confirmed;
static int done = 0;
static int quit = 0;

void* waiter(void *data) {
    const char* color = ANSI_GREEN;
    PRINT("start");

    pthread_mutex_lock(&mutex);
    PRINT("waiting");
    done = 0;
#ifdef WITH_TIMEOUT
    while (!done) {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        //now.tv_sec = now.tv_sec + 1;
        now.tv_nsec = now.tv_nsec + 100000000;
        if (now.tv_nsec > 1000000000) {
            now.tv_sec = now.tv_sec + 1;
            now.tv_nsec = now.tv_nsec - 1000000000;
        }

        int result = pthread_cond_timedwait(&confirmed, &mutex, &now);
        if (result != 0) PRINT("timeout");
        else break;
    }
#else
    while (!done) {
        int result = pthread_cond_wait(&confirmed, &mutex);
        if (result != 0) printf("result = %d\n", result);
    }
    if (quit) PRINT("aborting...");
#endif
    PRINT("waiting done");
    pthread_mutex_unlock(&mutex);

    PRINT("finish");
}

void* unlocker(void *data) {
    const char* color = ANSI_RED;
    PRINT("start");

    // TEMP
    for (int i=0; i<40; i++) {
        usleep(100000);
        if (quit) {
            PRINT("aborting");
            break;
        } 
    }

    PRINT("unlocking");
    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&confirmed);
    pthread_mutex_unlock(&mutex);
    PRINT("unlocking done");
    PRINT("finish");
}


void sigtrap(int) {
    printf("Ctrl-C\n");
    quit = 1;
}


int main() {
    pthread_t threads[2];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&confirmed, NULL);
    
    signal(SIGINT, sigtrap);

    pthread_create(&threads[0], 0, waiter, (void *)0);
    pthread_create(&threads[1], 0, unlocker, (void *)0);

    pthread_join(threads[0], 0);
    pthread_join(threads[1], 0);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&confirmed);


    return 0;
}

