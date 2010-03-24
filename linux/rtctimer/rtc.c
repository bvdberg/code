// compile with -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

volatile int running = 0;


#define FREQ 4  // should be power of 2
#define PERIOD 250000   // should be 1000000 / FREQ

void* worker_thread (void* cookie) {
    int* rtc_fd = (int*)cookie;
    unsigned long data;
    
    struct timeval now;
    u_int64_t now64 = 0;
    u_int64_t last64 = 0;
    u_int64_t start64 = 0;
    int count = 0;
    while (running)
    {
        read(*rtc_fd, &data, sizeof(unsigned long));
        gettimeofday(&now, 0);
        now64 = now.tv_sec * 1000000 + now.tv_usec;
        if (count == 0) start64 = now64;
        printf("%3d diff=%llu  absolute error=%lld (us)\n", count, now64 - last64, (now64 - count*PERIOD) - start64);
        last64 = now64;
        count++;
    }
    return 0;
}


int main(void) {
    int fd;
    fd = open ("/dev/rtc", O_RDONLY);
    if (fd < 0) {
        perror ("open failed");
        return -1;
    }

    printf("RTC_IRQP_READ\n");
    unsigned long freq = 0;
    if (ioctl(fd, RTC_IRQP_READ, &freq) < 0) {
        perror ("RTC_IRQP_READ ioctl failed");   //4 x per 1000/1024 seconde
        return -1;
    }
    printf("RTC_IRQP_READ freq=%lu\n", freq);
    sleep(1);


    printf("RTC_IRQP_SET\n");
    if (ioctl(fd, RTC_IRQP_SET, FREQ) < 0) {
        perror ("RTC_IRQP_SET ioctl failed");   //4 x per 1000/1024 seconde
        return -1;
    }
    sleep(1);
    /*
    printf("RTC_PIE_ON\n");
    if (ioctl(fd, RTC_PIE_ON, 0) < 0) {
        perror ("RTC_PIE_ON ioctl failed");
        return -1;
    }

    running = 1;
    pthread_t tid;

    pthread_create (&tid, 0, worker_thread, (void*)&fd);
    sleep (15);
    running = 0;

    pthread_join (tid, 0);

    ioctl(fd, RTC_PIE_OFF, 0);
    */
    close (fd);
    return 0;
}


