/* Copyright 2013-2024 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#include "events.h"

#define TIMER_PENDING 0x2

uint64_t current_time()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec / 1000);
    return now64;
}

static const char stop_cmd = 's';
//static const char update_cmd = 'r';

struct event_base_t_
{
    // loop control
    int running_loop;
    int loop_break;
    uint8_t return_value;
    int read_pipe;
    int write_pipe;
    // for select
    int max_fd;
    fd_set read_set;
    fd_set write_set;
    // events
    event_t* event_head;
    event_t* event_tail;
    etimer_t* timers;
};

static void events_sendcmd(event_base_t* base, char cmd)
{
    ssize_t written = write(base->write_pipe, &cmd, 1);
    (void)written;
}

static void timer_add_internal(etimer_t* timer)
{
    event_base_t* base = timer->base;

    const uint64_t timeout = timer->timeout;
    if (!base->timers)
    {
        // add as first, modify base timeout
        base->timers = timer;
    }
    else
    {
        if (timeout < base->timers->timeout)
        {
            // add before first, modify base timeout
            timer->next = base->timers;
            base->timers = timer;
        }
        else
        {
            // add sorted, dont modify base timeout
            etimer_t* cur = base->timers;
            while (cur->next)
            {
                if (cur->next->timeout > timeout) break;
                cur = cur->next;
            }
            timer->next = cur->next;
            cur->next = timer;
        }
    }
}

static void timer_fire(event_base_t* base)
{
    etimer_t* timer = base->timers;
    base->timers = timer->next;
    timer->next = 0;
    if (timer->flags & TIMER_PERSIST)
    {
        timer->timeout += timer->interval_usec;
        timer_add_internal(timer);
    }
    else
    {
        timer->flags &= ~TIMER_PENDING;
    }
    timer->handler(timer->handler_arg);
}

static void events_internal_loop(event_base_t* base, bool with_timers)
{
    struct timeval* timeout_ptr = NULL;
    struct timeval timeout;

    if (base->timers && with_timers)
    {
        etimer_t* timer = base->timers;
        uint64_t now = current_time();
        if (now >= timer->timeout)
        {
            timer_fire(base);
            return;
        }
        else
        {
            uint64_t delay = timer->timeout - now;
            timeout.tv_sec = delay / 1000000lu;
            timeout.tv_usec = (delay % 1000000lu);
            timeout_ptr = &timeout;
        }
    }

    int max_fd = base->max_fd;
    fd_set rdfs;
    fd_set wrfs;
    memcpy(&rdfs, &base->read_set, sizeof(fd_set));
    memcpy(&wrfs, &base->write_set, sizeof(fd_set));
    int res = select(max_fd + 1, &rdfs, &wrfs, NULL, timeout_ptr);
    if (res < 0) return;    // interrupted (by signal, etc)
    if (res == 0)   // timeout
    {
        etimer_t* timer = base->timers;
        if (timer)
        {
#if 0
            uint64_t now = current_time();
            / if (now < base->timers->timeout)
            {
                fprintf("TIMEOUT TOO EARLY %lu\n", base->timers->timeout - now);
            }
#endif
            timer_fire(base);
            return;
        }
    }
    if (res > 0)
    {
        if (FD_ISSET(base->read_pipe, &rdfs))
        {
            // only 1 command for now 's' = stop
            char cmd;
            ssize_t numread = read(base->read_pipe, &cmd, 1);
            (void)numread;
            return;
        }
        event_t* ev = base->event_head;
        while (ev)
        {
            if (FD_ISSET(ev->fd, &rdfs))
            {
                ev->handler(ev->fd, ev->handler_arg, EV_READ);
                return;
            }
            if (FD_ISSET(ev->fd, &wrfs))
            {
                ev->handler(ev->fd, ev->handler_arg, EV_WRITE);
                return;
            }
            ev = ev->next;
        }
    }
}

static void events_update_sets(event_base_t* base)
{
    FD_ZERO(&base->read_set);
    FD_ZERO(&base->write_set);

    int max_fd;
    max_fd = base->read_pipe;
    FD_SET(base->read_pipe, &base->read_set);

    event_t* ev = base->event_head;
    while (ev)
    {
        int fd = ev->fd;
        if (fd > max_fd) max_fd = fd;
        if (ev->flags & EV_READ) FD_SET(fd, &base->read_set);
        if (ev->flags & EV_WRITE) FD_SET(fd, &base->write_set);
        ev = ev->next;
    }
    base->max_fd = max_fd;
}

event_base_t* events_new()
{
    int pipefd[2];
    if (pipe2(pipefd, O_CLOEXEC | O_NONBLOCK) != 0)
    {
        return 0;
    }

    event_base_t* base = calloc(1, sizeof(event_base_t));

    base->read_pipe = pipefd[0];
    base->write_pipe = pipefd[1];

    events_update_sets(base);
    return base;
}

uint8_t events_mainloop(event_base_t* base)
{
    base->return_value = 0;
    base->running_loop = 1;
    while (!base->loop_break)
    {
        events_internal_loop(base, true);
    }
    base->running_loop = 0;
    return base->return_value;
}

void events_single_loop(event_base_t* base)
{
    base->running_loop = 1;
    if (!base->loop_break)
    {
        events_internal_loop(base, true);
    }
    base->running_loop = 0;
}
void events_single_loop_no_timers(event_base_t* base)
{
    base->running_loop = 1;
    if (!base->loop_break)
    {
        events_internal_loop(base, false);
    }
    base->running_loop = 0;
}

void events_free(event_base_t* base)
{
    close(base->read_pipe);
    close(base->write_pipe);
    free(base);
}

void events_loopbreak(event_base_t* base, uint8_t return_value)
{
    base->loop_break = 1;
    base->return_value = return_value;
    if (base->running_loop)
    {
        events_sendcmd(base, stop_cmd);
    }
}

int events_is_loopbroken(event_base_t* base)
{
    return base->loop_break;
}

int events_get_return_code(event_base_t* base)
{
    return base->return_value;
}

void events_assign(event_t* ev, event_base_t* base, int fd, short flags, event_func_t fn, void* arg)
{
    ev->fd = fd;
    ev->next = 0;
    ev->base = base;
    ev->handler =  fn;
    ev->handler_arg = arg;
    ev->flags = flags;
}

void events_add(event_t* ev)
{
    event_base_t* base = ev->base;

    if (base->event_tail)
    {
        base->event_tail->next = ev;
    }
    else
    {
        base->event_head = ev;
    }
    base->event_tail = ev;

    events_update_sets(base);
    // TODO trigger select? (only needed if we allow calling from other threads, also need lock then)
    //events_sendcmd(base, update_cmd);
}

void events_del(event_t* ev)
{
    assert(ev && ev->base);
    event_base_t* base = ev->base;

    int was_active = 0;
    if (base->event_head == ev)
    {
        // is first event
        base->event_head = ev->next;
        if (base->event_head == NULL) base->event_tail = NULL;
        was_active = 1;
    }
    else
    {
        event_t* cur = base->event_head;
        while (cur)
        {
            if (cur->next == ev)
            {
                cur->next = ev->next;
                if (base->event_tail == ev) base->event_tail = cur;
                was_active = 1;
                break;
            }
            cur = cur->next;
        }
    }
    if (was_active)
    {
        ev->next = 0;
        events_update_sets(base);
        // TODO trigger select? (only needed if we allow calling from other threads, also need lock then)
        //events_sendcmd(base, update_cmd);
    } // else was not added
}


void events_update_flags(event_t* ev, short flags)
{
    ev->flags = flags;
    events_update_sets(ev->base);
}

void timer_assign(etimer_t* timer, event_base_t* base, unsigned flags, timer_func_t fn, void* arg)
{
    timer->next = NULL;
    timer->timeout = 0;
    timer->base = base;
    timer->handler =  fn;
    timer->handler_arg = arg;
    timer->flags = flags;
    timer->interval_usec = 0;
}

int timer_active(etimer_t* timer)
{
    return (timer->flags & TIMER_PENDING) != 0;
}

void timer_add(etimer_t* timer, uint64_t delay_usec)
{
    assert(!timer_active(timer));

    uint64_t now = current_time();
    timer->interval_usec = delay_usec;
    timer->timeout = now + delay_usec;
    timer->flags |= TIMER_PENDING;
    timer->next = NULL;

    timer_add_internal(timer);
}

void timer_add_absolute(etimer_t* timer, uint64_t timeout)
{
    assert(!timer_active(timer));
    assert(!(timer->flags & TIMER_PERSIST));

    timer->interval_usec = 0;
    timer->timeout = timeout;
    timer->flags |= TIMER_PENDING;
    timer->next = NULL;

    timer_add_internal(timer);
}

void timer_del(etimer_t* timer)
{
    if (!timer_active(timer)) return;
    event_base_t* base = timer->base;
    assert(base && base->timers);

    if (timer == base->timers)
    {
        base->timers = timer->next;
    }
    else
    {
        etimer_t* cur = base->timers;
        while (cur->next)
        {
            if (cur->next == timer)
            {
                cur->next = timer->next;
                break;
            }
            cur = cur->next;
        }
    }
    timer->next = 0;
    timer->flags &= ~TIMER_PENDING;
}

