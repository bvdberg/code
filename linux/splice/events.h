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

#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

uint64_t current_time();

// NOTE: all functions should be called from same thread

typedef struct event_base_t_ event_base_t;

event_base_t* events_new();

uint8_t events_mainloop(event_base_t* base);

void events_single_loop(event_base_t* base);
void events_single_loop_no_timers(event_base_t* base);

void events_free(event_base_t* base);

void events_loopbreak(event_base_t* base, uint8_t return_value);

int events_is_loopbroken(event_base_t* base);

int events_get_return_code(event_base_t* base);

// events
typedef void (*event_func_t)(int fd, void* arg, short flags);

struct event_t_;
typedef struct event_t_ event_t;

// struct fields are for internal use only. Others should consider this an opaque struct
struct event_t_
{
    int fd;
    event_t* next;
    event_base_t* base;
    event_func_t handler;
    void* handler_arg;
    short flags;
};

#define EV_READ 0x02
#define EV_WRITE 0x04

void events_assign(event_t* ev, event_base_t* base, int fd, short flags, event_func_t fn, void* arg);

void events_add(event_t* ev);

void events_del(event_t* ev);

// may be done while event is active
void events_update_flags(event_t* ev, short flags);

// timers
struct etimer_t_;
typedef struct etimer_t_  etimer_t;

typedef void (*timer_func_t)(void* arg);

// struct fields are for internal use only. Others should consider this an opaque struct
struct etimer_t_
{
    etimer_t* next;
    uint64_t timeout;
    event_base_t* base;
    timer_func_t handler;
    void* handler_arg;
    uint64_t interval_usec;
    unsigned flags;
};

#define TIMER_PERSIST 0x1
void timer_assign(etimer_t* timer, event_base_t* base, unsigned flags, timer_func_t fn, void* arg);

int timer_active(etimer_t* timer);

void timer_add(etimer_t* timer, uint64_t delay_usec);

void timer_add_absolute(etimer_t* timer, uint64_t timeout);

void timer_del(etimer_t* timer);

#define SEC(x) ((x) * 1000000u)
#define MSEC(x) ((x) * 1000u)

#ifdef __cplusplus
}
#endif

#endif

