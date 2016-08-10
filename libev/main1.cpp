#include "ev.h"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

// NOTE: Hardcoded libev.a binary for Ubuntu 14.04 64bit

ev_io stdin_watcher;
ev_timer timeout_watcher;
ev_periodic timer2;
struct ev_loop *loop = EV_DEFAULT;

bool small = true;

static ev_tstamp last = 0;
static uint64_t last64;


struct MyData {
    int num;
};

static uint64_t getCurrentTime() {
#if 1
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec/1000);
    return now64;
#endif
}

static void periodic_cb (struct ev_loop *loop, ev_periodic *w, int revents)
{
    //uint64_t now64 = getCurrentTime();
    //uint64_t diff64 = now64 - last64;
    // Conclusion: err differs from err64 <= 1usec
    //const MyData* d = (MyData*)w->data;
    //printf("periodic data = %p (%d)\n", d, d->num);
    ev_tstamp now = ev_time();
    ev_tstamp diff = now - last;
    if (last == 0)  diff = 0;
    ev_tstamp err = diff - 0.100000;
    //printf("now=%lf  ", ev_now(loop));
    printf("periodic  %lf   %lf   %lf\n", now, diff, err);
    //printf("periodic  %lf   %lf   %lf   %lu\n", now, diff, err, diff64);
    last = now;
    //last64 = now64;
}

static ev_tstamp my_scheduler_cb (ev_periodic *w, ev_tstamp now)
{
    printf("scheduler\n");
    if (small) {
        small = false;
        return now + 1.0;
    } else {
        small = true;
        return now + 0.2;
    }
}

static void stdin_cb (EV_P_ ev_io *w, int revents)
{
    puts ("stdin ready");
    // for one-shot events, one must manually stop the watcher
    // with its corresponding stop function.
    ev_io_stop (EV_A_ w);

    ev_break (EV_A_ EVBREAK_ALL);
}

static void timeout_cb (EV_P_ ev_timer *w, int revents)
{
    //const MyData* d = (MyData*)w->data;
    //printf("single-shot data = %p (%d)\n", d, d->num);
    puts ("timeout");
    ev_break (EV_A_ EVBREAK_ONE);
}

static void sigint_cb (struct ev_loop *loop, ev_signal *w, int revents)
{
    printf("Ctrl-C\n");
    ev_break (loop, EVBREAK_ALL);
}

int main (void)
{
    MyData mydata = { 123 };
    MyData mydata2 = { 234 };
    //printf("MYDATA:  %p   %p\n", &mydata, &mydata2);

    printf("supported modes:");
    if (ev_supported_backends () & EVBACKEND_SELECT) printf("  SELECT");
    if (ev_supported_backends () & EVBACKEND_POLL) printf("  POLL");
    if (ev_supported_backends () & EVBACKEND_EPOLL) printf("  EPOLL");
    printf("\n");

    ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
    ev_io_start (loop, &stdin_watcher);

    // single-shot timer
    ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
    timeout_watcher.data = &mydata2;
    ev_timer_start (loop, &timeout_watcher);

    // periodic timer
    ev_periodic_init (&timer2, periodic_cb, 0., 0.1, 0);
    // pass void* arg to callback function -> every watcher has a 'void* data' member
    timer2.data = &mydata;
    //ev_periodic_init (&timer2, periodic_cb, 0., 0.5, my_scheduler_cb);   // uses callback for new time
    //printf("pending=%d  active=%d\n", ev_is_pending(&timer2), ev_is_active(&timer2));
    ev_periodic_start (loop, &timer2);
    //printf("pending=%d  active=%d\n", ev_is_pending(&timer2), ev_is_active(&timer2));

    // handle Ctrl-C properly
    ev_signal signal_watcher;
    ev_signal_init (&signal_watcher, sigint_cb, SIGINT);
    ev_signal_start (loop, &signal_watcher);

    // TODO stop/restart timer (from timer?)

    ev_run (loop, 0);

    return 0;
}

