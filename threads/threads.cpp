// compile with -lpthread

#include <stdio.h>
#include <stdlib.h> //for exit
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "color.h"


static const int TIMES = 2;

class Buffer {
public:
    Buffer(int i) : count(i) {}
    //TODO add buffering
    void get() {
        count--;
        printf("Buffer: get %d \n", count);
    }
    void put() {
        count++;
        printf("Buffer: put %d \n", count);
    }
private:
    int count;    
};


class Mover {
public:
    Mover(Buffer* l, Buffer* r) 
        : left(l), right(r)
    {}
    virtual void start() = 0;
protected:
    Buffer* left;
    Buffer* right;
};


//static method
void* startThread(void* mover)
{
    Mover* m = reinterpret_cast<Mover*>(mover);
    assert(m);
    printf(" thread starting pid=%d  self=%lu\n", getpid(), pthread_self());
    m->start();
    sleep(200);
    printf(" thread finished\n");
    return 0;
}


class LeftToRight : public Mover {
public:
    LeftToRight(Buffer* l, Buffer* r) : Mover(l, r) {}
    void start() {
        printf(ANSI_GREEN"LeftToRight started"ANSI_NORMAL"\n");
        for(int i=0; i<TIMES; i++) {
            printf(ANSI_GREEN"moving L->R (%d)"ANSI_NORMAL"\n", i);
            left->get();
            right->put();
            struct timeval waitTime;
            waitTime.tv_sec = 0;
            waitTime.tv_usec = 1000;
            select(0, 0, 0, 0, &waitTime);
        }
    }
};
    
    
class RightToLeft : public Mover {
public:
    RightToLeft(Buffer* l, Buffer* r) : Mover(l, r) {}
    void start() {
        printf(ANSI_RED"RightToLeft started"ANSI_NORMAL"\n");
        for(int i=0; i<TIMES; i++) {
            printf(ANSI_RED"moving R->L (%d)"ANSI_NORMAL"\n", i);
            right->get();
            left->put();
            struct timeval waitTime;
            waitTime.tv_sec = 0;
            waitTime.tv_usec = 1000;
            select(0, 0, 0, 0, &waitTime);
        }
    }

};


int main() {

    Buffer buffer1(10);
    Buffer buffer2(10);
    LeftToRight l2r(&buffer1, &buffer2);
    RightToLeft r2l(&buffer1, &buffer2);

    pthread_t threads[2];
    
    pid_t pid = getpid();
    printf("main pid=%d\n", pid);
    int rc = pthread_create(&threads[0], 0, startThread, (void *)&l2r);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(1);
    }
    rc = pthread_create(&threads[1], 0, startThread, (void *)&r2l);
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

