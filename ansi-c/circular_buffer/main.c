#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "circbuf.h"

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
#define ANSI_NORMAL "\033[22;0m"

static int producer_delay = 12000;
static int consumer_delay = 15000;

void* producer(void* ptr)
{
    Buffer* buf = (Buffer*)ptr;
    while (1) {
        int amount = rand() % 5 + 1;
        int numFree = buffer_free(buf);
        amount = 1; // TEMP
        //printf(ANSI_GREEN"producer: t=%d  p=%d:  amount=%d  free=%d"ANSI_NORMAL"\n", 
        //        buf->takeIndex, buf->putIndex, amount, numFree);
        if (numFree < amount) {
            printf(ANSI_GREEN"producer: no free space left in queue...wating"ANSI_NORMAL"\n");
        } else {
            buffer_add(buf, amount);
        }
        buffer_print(buf);
        fflush(stdout);
        usleep(producer_delay);
    }

    return 0;
}


void* consumer(void* ptr)
{
    Buffer* buf = (Buffer*)ptr;
    while (1) {
        int numData = buffer_data(buf);
        //printf(ANSI_RED"consumer: t=%d  p=%d  numData=%d"ANSI_NORMAL"\n",
        //    buf->takeIndex, buf->putIndex, numData);
        if (numData >= 1) buffer_remove(buf, 1);
        if (numData == 18) consumer_delay = 11000;
        if (numData == 0) consumer_delay = 15000;
        usleep(consumer_delay);
    }

    return 0;
}


int main() {

    srand(time(0));
    Buffer buffer;
    buffer_init(&buffer);

    pthread_t threads[2];
    
    int rc = pthread_create(&threads[0], 0, producer, (void *)&buffer);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(1);
    }

    rc = pthread_create(&threads[1], 0, consumer, (void *)&buffer);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(1);
    }


    pthread_join(threads[0], 0);
    threads[0] = 0;
    pthread_join(threads[1], 0);
    threads[1] = 0;

    return 0;
}

