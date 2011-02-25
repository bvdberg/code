#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "rdtsc.h"
// cat/proc/cpuinfo shows timebase of 66660000
static const unsigned long long TIMEBASE_SEC = 66660000;
static const unsigned long long TIMEBASE_MS = 66660;
static const unsigned long long TIMEBASE_USEC = 67; // NOTE rounding errors

static const int MAX_BUFFER = 24;

static char* reason = "unknown";
static int stop = 0;
static int count = 0;

static void sigint(int signum)
{
    reason = "Ctrl-C";
    stop = 1;
}


static void sigterm(int signum)
{
    reason = "TERM signal";
    stop = 1;
}


static const char* duration2str(unsigned long long ticks)
{
    static int index = 0;
    static char result[4][32];
    index++;
    if (index > 3) index = 0;
    snprintf(result[index], 31, "%llu usec", ticks / TIMEBASE_USEC);
    result[index][31] = 0;
    return result[index];
}


static void usage()
{
    fprintf(stderr, "usage: timertest [fifo] [loops]\n");
    exit(1);
}


int main (int argc, char* argv[]) {
    if (argc != 3) usage();

    const char* name = argv[1];
    count = atoi(argv[2]);

    signal(SIGINT, sigint);
    signal(SIGTERM, sigterm);

    int clock_id = 1;
    struct sigevent evp;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGUSR2;
    int qi = 10; // ?
    evp.sigev_value.sival_int=qi;
    int err = timer_create(clock_id, &evp, (timer_t*) 


    // TODO rtimetable_control() ?
    struct itimerspec value;
    memset(&value, 0, sizeof(value));
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    value.it_value.tv_sec = 1;
    value.it_value.tv_nsec = 0;
#define TIMER_RELTIME 0x0
    // TIMER_ABSTIME
    int timer = 10;
    printf("setting timer\n");
    unsigned long long t1 = rdtsc();
    int err = timer_settime((timer_t)timer, TIMER_RELTIME, &value, NULL);
    if (err) {
        perror("timer_settime");
        exit(1);
    }
    unsigned long long t2 = rdtsc();
    printf("timer returned, duration %s\n", duration2str(t2-t1));


	return 0;
}

