#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <pthread.h>
#include <sys/time.h>
#include "myassert.h"

/*

    Design notes:
    - execution of a task should never be preempted on a multicore. Interrupts may
      still occur.
    - Critical sections of same lock run on one thread/cpu, thus a lock has affinity
      to a cpu/thread. Non-critical sections can still run anywhere.

    To be able to do real time:
    - the compiler should be able to calculate whether a given
      program with the time constraints can work on given hardware.
    - compiler should calculate how long tasks take (for scheduler)
    - the max time of each task in the system should be known a priori

    goal is to meet deadlines, not lower average response time for real-time
    tasks!

    Idea:
    - add a maximum delay attribute to tasks. This is the latest time it should
        be scheduled.
    - add channels -> Task->Task communication.

    Tasks are in a list:
    - runnable (TaskQueue)
    - waiting for lock (Lock list)
    - running (in TaskRunner thread)

    --------------
    TODO:
    - delay Reader/Writer. Only let it run every so period
    - priorities (hi/low)

    TODO:
    - multiple reader, single writer lock

    NOTE:
    - this method doesn't work on NUMA machines, where a task also has associated
        memory (and CPU dependent registers)
*/

u_int64_t getCurrentTime();


class Task {
public:
    Task(char* name_);
    virtual ~Task() {}

    virtual bool run(char* color) = 0; // bool = re-schedule
    Task* next;
    char* getName() const { return name; }
protected:
    char* name;
private:
    Task(const Task&);
    Task& operator=(const Task&);
};


class TaskLock {
public:
    TaskLock();
    ~TaskLock();
    void lock(Task* current, Task* lockTask);
    void unlock();
private:
    void addToWaiters(Task* t);

    Task* waiters;
    pthread_mutex_t plock;
    bool locked;
};


struct DelayedTask {
    u_int64_t time;
    Task* task;
    DelayedTask* next;
    DelayedTask(u_int64_t time_, Task* task_) : time(time_), task(task_), next(0) {}
};


class MultipleReaderLock {
public:
    MultipleReaderLock();
    ~MultipleReaderLock();
    void reader_lock(Task* current, Task* lockTask);
    void reader_unlock();
    void writer_lock(Task* current, Task* lockTask);
    void writer_unlock();
private:
    void addToWaitingReaders(Task* t);
    void addToWaitingWriters(Task* t);
    void scheduleReader();
    void scheduleWriter();

    Task* waitingWriters;
    Task* waitingReaders;
    pthread_mutex_t plock;
    int readers;
    bool locked;
};


class TaskQueue {
public:
    TaskQueue();
    ~TaskQueue();

    void addTask(Task* task);
    void addDelayedTask(u_int64_t time, Task* task);
    Task* getTask();
    u_int64_t maxInterval;
private:
    // these vars are synchronized
    Task* firstLow;
    Task* lastLow; 
    DelayedTask* delayedTask;
    u_int64_t lastNow;

    pthread_mutex_t lock;

    TaskQueue(const TaskQueue&);
    TaskQueue& operator=(const TaskQueue&);
};

extern TaskQueue* taskQueue;

#endif

