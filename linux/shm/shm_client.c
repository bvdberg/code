#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    const key_t key = 5678;
    int size = 1000;

    int shmid;
    if ((shmid = shmget(key, size, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Now we attach the segment to our data space.
    char* shm;
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    printf("mapped to %p\n", shm);


    
    char* c = shm;
    while (c < shm + size) {
        putchar(*c);
        c++;
    }
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';

    shmdt(shm); 

    return 0;
}

