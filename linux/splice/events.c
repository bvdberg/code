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
#include <stdio.h>
#include <inttypes.h>

#include "events.h"

#define TIMER_PENDING 0x2

#define EVENT_PENDING 0x1

uint64_t events_now()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec / 1000);
    return now64;
}

typedef enum
{
    CMD_LOOPBREAK = 0,
    CMD_CALLBACK,
    CMD_TIMER_ADD,
    CMD_TIMER_DEL,
} cmd_type_t;

typedef struct __attribute__((__packed__))
{
    cmd_type_t type;
    union {
        struct {
            events_cb_t func;
            void* arg1;
            void* arg2;
        } callback;
        struct {
            etimer_t* timer;
            uint64_t delay_us;
        } timer;
    };
} event_cmd_t;

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
    struct list_tag event_list;
    struct list_tag timer_list;
};

static void events_send_cmd(event_base_t* base, const event_cmd_t* cmd)
{
    ssize_t written = write(base->write_pipe, cmd, sizeof(event_cmd_t));
    assert(written == sizeof(event_cmd_t));
    (void)written;
}

static void timer_add_internal(etimer_t* timer)
{
    event_base_t* base = timer->base;

    const uint64_t timeout = timer->timeout;
    if (list_empty(&base->timer_list))
    {
        list_add_front(&base->timer_list, &timer->list);
    }
    else
    {
        // add sorted
        list_t node = base->timer_list.next;
        while (node != &base->timer_list)
        {
            etimer_t *cur = to_container(node, etimer_t, list);
            if (timeout < cur->timeout) {
                list_add_tail(&cur->list, &timer->list);
                return;
            }

            node = node->next;
        }
        list_add_tail(&base->timer_list, &timer->list);
    }
}

static void timer_fire(event_base_t* base)
{
    list_t node = list_pop_front(&base->timer_list);
    etimer_t *timer = to_container(node, etimer_t, list);

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

    if (!list_empty(&base->timer_list) && with_timers)
    {
        etimer_t *timer = to_container(base->timer_list.next, etimer_t, list);
        uint64_t now = events_now();
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
        if (!list_empty(&base->timer_list))
        {
#if 0
            uint64_t now = events_now();
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
            event_cmd_t cmd;
            ssize_t numread = read(base->read_pipe, &cmd, sizeof(event_cmd_t));
            assert(numread == sizeof(cmd));
            (void)numread;
            switch (cmd.type)
            {
                case CMD_LOOPBREAK:
                    break;
                case CMD_CALLBACK:
                    cmd.callback.func(cmd.callback.arg1, cmd.callback.arg2);
                    break;
                case CMD_TIMER_ADD:
                    timer_add(cmd.timer.timer, cmd.timer.delay_us);
                    break;
                case CMD_TIMER_DEL:
                    timer_del(cmd.timer.timer);
                    break;
            }
            return;
        }


        list_t node = base->event_list.next;

        while (node != &base->event_list)
        {
            event_t *ev = to_container(node, event_t, list);
            if (FD_ISSET(ev->fd, &rdfs))
            {
                // move to last to implement round-robin scheduling
                list_remove(&ev->list);
                list_add_tail(&base->event_list, &ev->list);
                ev->handler(ev->fd, ev->handler_arg, EV_READ);
                return;
            }
            if (FD_ISSET(ev->fd, &wrfs))
            {
                list_remove(&ev->list);
                list_add_tail(&base->event_list, &ev->list);
                ev->handler(ev->fd, ev->handler_arg, EV_WRITE);
                return;
            }
            node = node->next;
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

    list_t node = base->event_list.next;
    while (node != &base->event_list)
    {
        event_t *ev = to_container(node, event_t, list);
        int fd = ev->fd;
        if (fd > max_fd) max_fd = fd;
        if (ev->flags & EV_READ) FD_SET(fd, &base->read_set);
        if (ev->flags & EV_WRITE) FD_SET(fd, &base->write_set);
        node = node->next;
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

    list_init(&base->event_list);
    list_init(&base->timer_list);

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
    base->loop_break = 0;
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
    if (base->loop_break) return;
    base->loop_break = 1;
    base->return_value = return_value;
    if (base->running_loop)
    {
        event_cmd_t cmd = { 0  };
        cmd.type = CMD_LOOPBREAK;
        events_send_cmd(base, &cmd);
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
    ev->base = base;
    ev->handler =  fn;
    ev->handler_arg = arg;
    ev->flags = flags;
    list_init(&ev->list);
}

void events_add(event_t* ev)
{
    if (ev->flags & EVENT_PENDING) abort();
    event_base_t* base = ev->base;
    list_add_front(&base->event_list, &ev->list);
    ev->flags |= EVENT_PENDING;
    events_update_sets(base);
}

void events_del(event_t* ev)
{
    assert(ev && ev->base);
    if ((ev->flags & EVENT_PENDING) != 0)
    {
        event_base_t* base = ev->base;
        list_remove(&ev->list);
        ev->flags &= ~EVENT_PENDING;
        events_update_sets(base);
    }
}

void events_update_flags(event_t* ev, short flags)
{
     short active = ev->flags & EVENT_PENDING;
    ev->flags = flags | active;
    events_update_sets(ev->base);
}

void timer_assign(etimer_t* timer, event_base_t* base, const char* name, unsigned flags, timer_func_t fn, void* arg)
{
    timer->timeout = 0;
    timer->base = base;
    timer->handler =  fn;
    timer->handler_arg = arg;
    timer->flags = flags;
    timer->interval_usec = 0;
    strncpy(timer->name, name, sizeof(timer->name));
    list_init(&timer->list);
}

void timer_dump(event_base_t* base)
{
    list_t node = base->timer_list.next;

    while (node != &base->timer_list)
    {
      etimer_t *timer = to_container(node, etimer_t, list);
      fprintf(stderr, "Timer [%16s] flag: %d timeout %"PRIu64" interval_us %"PRIu64" \n", timer->name, timer->flags, timer->timeout, timer->interval_usec);
      node = node->next;
    }
}

int timer_active(etimer_t* timer)
{
    return (timer->flags & TIMER_PENDING) != 0;
}

void timer_add(etimer_t* timer, uint64_t delay_usec)
{
    assert(!timer_active(timer));

    uint64_t now = events_now();
    timer->interval_usec = delay_usec;
    timer->timeout = now + delay_usec;
    timer->flags |= TIMER_PENDING;

    timer_add_internal(timer);
}

void timer_add_offset(etimer_t* timer, uint64_t delay_usec, uint64_t offset)
{
    assert(!timer_active(timer));

    uint64_t now = events_now();
    timer->interval_usec = delay_usec;
    timer->timeout = now + delay_usec + offset;
    timer->flags |= TIMER_PENDING;

    timer_add_internal(timer);
}

void timer_add_absolute(etimer_t* timer, uint64_t timeout)
{
    assert(!timer_active(timer));
    assert(!(timer->flags & TIMER_PERSIST));

    timer->interval_usec = 0;
    timer->timeout = timeout;
    timer->flags |= TIMER_PENDING;

    timer_add_internal(timer);
}

void timer_del(etimer_t* timer)
{
    if (!timer_active(timer)) return;

    list_remove(&timer->list);
    timer->flags &= ~TIMER_PENDING;
}

void events_schedule_safe(event_base_t* base, events_cb_t func, void* arg1, void* arg2)
{
    event_cmd_t cmd = { 0 };
    cmd.type = CMD_CALLBACK;
    cmd.callback.func =  func;
    cmd.callback.arg1 = arg1;
    cmd.callback.arg2 = arg2;
    events_send_cmd(base, &cmd);
}

void timer_add_safe(etimer_t* timer, uint64_t delay_usec)
{
    event_base_t* base = timer->base;
    assert(base);

    event_cmd_t cmd = { 0 };
    cmd.type = CMD_TIMER_ADD;
    cmd.timer.timer = timer;
    cmd.timer.delay_us = delay_usec;
    events_send_cmd(base, &cmd);
}

void timer_del_safe(etimer_t* timer)
{
    event_base_t* base = timer->base;
    assert(base);

    event_cmd_t cmd = { 0 };
    cmd.type = CMD_TIMER_DEL;
    cmd.timer.timer = timer;
    events_send_cmd(base, &cmd);
}

