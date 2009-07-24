#include <stdio.h>
#include <stdlib.h> // for exit
#include <unistd.h> // for sleep
#include <assert.h>

#include "color.h"
#include "Manager.h"

// cleanup method
void cleanup1(void *param_)
{
    Param* param = reinterpret_cast<Param *>(param_);
    assert(param);
    
    printf("cleanup1() called for thread %d\n", param->id);
}


void cleanup2(void *param_)
{
    Param* param = reinterpret_cast<Param *>(param_);
    assert(param);
    
    printf("cleanup2() called for thread %d\n", param->id);
}


// static method
void* startThread(void *param_)
{
    Param* param = reinterpret_cast<Param *>(param_);
    assert(param);

    if(param->id == 0) printf(ANSI_RED);
    else printf(ANSI_GREEN);
    printf("thread %d starting"ANSI_NORMAL"\n", param->id);

    pthread_cleanup_push(cleanup1, param_);
    pthread_cleanup_push(cleanup2, param_);

    while(!param->stop) {
        if(param->id == 0) printf(ANSI_RED);
    	else printf(ANSI_GREEN);
	    printf("thread %d sleeping."ANSI_NORMAL"\n", param->id);
    	sleep(1);
    }

    if(param->id == 0) printf(ANSI_RED);
    else printf(ANSI_GREEN);
    printf("thread %d: finished"ANSI_NORMAL"\n", param->id);

    pthread_cleanup_pop(1);	//1 runs method, 0 doesn't
    pthread_cleanup_pop(1);	//1 runs method, 0 doesn't
    return 0;
}


Manager::Manager()
{
   threads[0] = 0;
   threads[1] = 0;
   params[0] = new Param(0);
   params[1] = new Param(1);
}


Manager::~Manager()
{
    printf("Manager::~Manager()\n");
    if(threads[0] || threads[1])
    {
        printf("stopping threads first\n");
        stop();
    }

    delete params[0];
    delete params[1];    
}


void Manager::start()
{
    for(int t=0;t < 2;t++){
        int rc = pthread_create(&threads[t], 0, startThread, (void *)params[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(1);
        }
    }
}


void Manager::stop()
{
    printf("Manager::stop()\n");
    params[0]->stop = true;
    params[1]->stop = true;

    int res;
    res = pthread_join(threads[0], 0);
    printf("thread 0 joined (res = %d)\n", res);
    res = pthread_join(threads[1], 0);
    printf("thread 1 joined (res = %d)\n", res);

    threads[0] = 0;
    threads[1] = 0;
}


void Manager::cancel()
{
    printf("Manager::cancel()\n");
    pthread_cancel(threads[0]);
    pthread_cancel(threads[1]);

    int res;
    res = pthread_join(threads[0], 0);
    printf("thread 0 joined (res = %d)\n", res);
    res = pthread_join(threads[1], 0);
    printf("thread 1 joined (res = %d)\n", res);

    threads[0] = 0;
    threads[1] = 0;
}

