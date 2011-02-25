#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int stop = 0;

void sigint(int num)
{
    stop = 1;
}

int main()
{
    const key_t key = 5678;

    int size = 1000;
    int shmid;
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    printf("shmid=%d\n", shmid);

    // Now we attach the segment to our data space.
    char* shm = NULL; 
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    printf("mapped to %p\n", shm);

    signal(SIGINT, sigint);

    // Now put some things into the memory for the other process to read.
    char *c = shm;
    while (c < shm + size) {
        char cc = ((int)c & 0xf) + 'a';
        *c = cc;
        c++;
    }

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (!stop && *shm != '*') sleep(1);

    printf("cleaning memory\n");
    shmdt(shm); 
    shmctl(shmid, IPC_RMID, 0); 
    return 0;
}

