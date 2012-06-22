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

#include "triple_buf.h"

#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_NORMAL "\033[22;0m"

static int producer_delay = 10000;
static int consumer_delay = 10000;

static int produce_duration = 19000;
static int consume_duration = 20000;

void* producer(void* ptr)
{
    Buffer* buf = (Buffer*)ptr;
    while (1) {
        int slot = write_start(buf);
        if (slot == -1) {
            printf(ANSI_GREEN"producer: no free space left in queue...waiting"ANSI_NORMAL"\n");
            buffer_print(buf);
            exit(-1);
            usleep(producer_delay);
            continue;
        }
        
        //printf(ANSI_GREEN"producer: slot %d"ANSI_NORMAL"\n", slot);
        buffer_print(buf);
        usleep(produce_duration);
        write_done(buf);
        buffer_print(buf);
    }

    return 0;
}


void* consumer(void* ptr)
{
    Buffer* buf = (Buffer*)ptr;
    while (1) {
        int slot = read_start(buf);
        if (slot == -1) {
            printf(ANSI_RED"consumer: no data in queue...waiting"ANSI_NORMAL"\n");
            usleep(consumer_delay);
            continue;
        }
        
        //printf(ANSI_RED"consumer: slot %d"ANSI_NORMAL"\n", slot);
        buffer_print(buf);
        usleep(consume_duration);
        read_done(buf);
        buffer_print(buf);
    }

    return 0;
}


int main() {

    srand(time(0));
    Buffer buffer;
    buffer_init(&buffer);

    pthread_t threads[2];
    
    // TODO change update speeds of reader/writer
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

