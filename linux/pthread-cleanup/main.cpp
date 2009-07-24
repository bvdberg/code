#include <stdio.h>
#include <unistd.h> //for sleep

#include "Manager.h"

int main (int argc, char *argv[])
{
    Manager manager;
    printf("main: starting manager\n");
    manager.start();
    printf("main: sleeping\n");
    sleep(5);
    printf("main: stopping manager\n");
    manager.stop();		// cleanup() not called (normal exit)
    //manager.cancel();		// cleanup() called (cancellation)

    printf("main: all done\n");
}

