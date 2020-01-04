#include <stdio.h>
#include <sys/wait.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>

#define UNUSED(x) (void)(x)
const unsigned TIMES=10000;

static uint64_t now()
{
	struct timeval val;
	int err = gettimeofday(&val, NULL);
	if (err) return 0;
	uint64_t result = val.tv_sec * 1000000;
	result += val.tv_usec;
	return result;
}

static void create_processes() {
    uint64_t t1 = now();
    for (uint32_t i=0; i<TIMES; ++i) {
        pid_t pid = fork();
        if (pid == 0) { // child
            exit(EXIT_SUCCESS);
        }
#if 1
    // also wait
        int wstatus = 0;
        if (waitpid(pid, &wstatus, 0) == -1) {
            printf("error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
#endif
    }
    uint64_t t2 = now();
    uint64_t duration = t2 - t1;
    printf("process creation (%u times) = %lu  (%lu usec each)\n", TIMES, duration, duration/TIMES);
}

static void* thread_main(void* arg) {
    UNUSED(arg);
    return NULL;
}

static void create_threads() {
    uint64_t t1 = now();
    for (uint32_t i=0; i<TIMES; ++i) {
        pthread_t thread;
        if (pthread_create(&thread, 0, thread_main, NULL) == -1) {
            fprintf(stderr, "error creating thread: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        void* retval;
        if (pthread_join(thread, &retval) == -1) {
            fprintf(stderr, "error voiding thread: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    uint64_t t2 = now();
    uint64_t duration = t2 - t1;
    printf(" thread creation (%u times) = %lu  (%lu usec each)\n", TIMES, duration, duration/TIMES);
}


int main()
{
    create_processes();
    create_threads();

    create_processes();
    create_threads();
	return 0;
}

