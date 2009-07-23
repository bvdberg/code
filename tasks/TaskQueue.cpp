#include "myassert.h"

#include "TaskQueue.h"

using namespace Mud;

u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
}

// ---------- Task ----------

Task::Task(char* name_) : next(0), name(name_)
{}

// ---------- TaskLock ----------

#define TASK_LOCK pthread_mutex_lock(&plock)
#define TASK_UNLOCK pthread_mutex_unlock(&plock)

TaskLock::TaskLock() 
    : waiters(0)
    , locked(false)
{
    pthread_mutex_init(&plock, 0);
}

TaskLock::~TaskLock() {
    while (waiters) {
        Task* tmp = waiters;
        waiters = waiters->next;
        delete tmp;
    }
}

void TaskLock::lock(Task* current, Task* lockTask) {
    TASK_LOCK;
    if (locked) {
        addToWaiters(lockTask);
    } else {
        locked = true;
        ASSERT_NULL(current->next);
        current->next = lockTask;
    }
    TASK_UNLOCK;
}

void TaskLock::unlock() {
    TASK_LOCK;
    ASSERT_TRUE(locked);
    if (waiters) {  // NOTE: stay locked, schedule first waiting task
        Task* tmp = waiters;
        waiters = waiters->next;
        tmp->next = 0;
        taskQueue->addTask(tmp);
    } else {
        locked = false;
    }
    TASK_UNLOCK;
}

void TaskLock::addToWaiters(Task* task) {
    ASSERT_NULL(task->next);
    task->next = 0;
    if (waiters == 0) {
        waiters = task;
        return;
    }
    Task* tmp = waiters;
    while (tmp->next) tmp = tmp->next;
    tmp->next = task;
}

// ---------- MultipleReaderLock ----------

MultipleReaderLock::MultipleReaderLock()
    : waitingWriters(0)
    , waitingReaders(0)
    , readers(0)
    , locked(false)
{
    pthread_mutex_init(&plock, 0);
}

MultipleReaderLock::~MultipleReaderLock() {
    // TODO cleanup
}

void MultipleReaderLock::reader_lock(Task* current, Task* lockTask) {
    TASK_LOCK;
    if (waitingWriters || locked) {
        addToWaitingReaders(lockTask);
    } else {
        readers++;
        current->next = lockTask;
    }
    TASK_UNLOCK;
}

void MultipleReaderLock::reader_unlock() {
    TASK_LOCK;
    ASSERT_FALSE(locked);
    readers--;

    if (readers==0 && waitingWriters) {
        locked = true;
        scheduleWriter();
    }
    TASK_UNLOCK;
}

void MultipleReaderLock::writer_lock(Task* current, Task* lockTask) {
    TASK_LOCK;
    if (locked || readers) {
        addToWaitingWriters(lockTask);
    } else {
        locked = true;
        ASSERT_NULL(current->next);
        current->next = lockTask;
    }
    TASK_UNLOCK;
}

void MultipleReaderLock::writer_unlock() {
    TASK_LOCK;
    ASSERT_TRUE(locked);
    ASSERT_EQUAL(0, readers);

    if (waitingWriters) {       // Writers get first
        scheduleWriter();
    } else {
        locked = false;
        while (waitingReaders) scheduleReader();
    }
    TASK_UNLOCK;
}

void MultipleReaderLock::scheduleWriter() {
    Task* tmp = waitingWriters;
    waitingWriters = tmp->next;
    tmp->next = 0;
    taskQueue->addTask(tmp);
}

void MultipleReaderLock::scheduleReader() {
    readers++;
    Task* tmp = waitingReaders;
    waitingReaders = tmp->next;
    tmp->next = 0;
    taskQueue->addTask(tmp);
}

void MultipleReaderLock::addToWaitingReaders(Task* task) {
    ASSERT_NULL(task->next);
    task->next = 0;
    if (waitingReaders == 0) {
        waitingReaders = task;
        return;
    }
    Task* tmp = waitingReaders;
    while (tmp->next) tmp = tmp->next;
    tmp->next = task;
}

void MultipleReaderLock::addToWaitingWriters(Task* task) {
    ASSERT_NULL(task->next);
    task->next = 0;
    if (waitingWriters == 0) {
        waitingWriters = task;
        return;
    }
    Task* tmp = waitingWriters;
    while (tmp->next) tmp = tmp->next;
    tmp->next = task;
}

// ---------- TaskQueue ----------

TaskQueue::TaskQueue() 
    : maxInterval(0)
    , firstLow(0)
    , lastLow(0)
    , delayedTask(0)
    , lastNow(getCurrentTime())
{
    pthread_mutex_init(&lock, 0);
}

TaskQueue::~TaskQueue() {
    while (firstLow) {
        Task* tmp = firstLow;
        firstLow = firstLow->next;
        delete tmp;
    }
}

#define LOCK pthread_mutex_lock(&lock)
#define UNLOCK pthread_mutex_unlock(&lock)

void TaskQueue::addTask(Task* newTask) {
    ASSERT_NULL(newTask->next);
    newTask->next = 0;
    LOCK;
    if (!firstLow) firstLow = newTask;
    else lastLow->next = newTask;
    lastLow = newTask;
    UNLOCK;
}


void TaskQueue::addDelayedTask(u_int64_t time, Task* task) {
    DelayedTask* dt = new DelayedTask(time, task);
    LOCK;
    if (delayedTask==0) {       // start new list
        delayedTask = dt;
        UNLOCK;
        return;
    }
    if (time < delayedTask->time) {     // add to front, before 1st element
        dt->next = delayedTask;
        delayedTask = dt;
        UNLOCK;
        return;     
    }
    DelayedTask* current = delayedTask;
    while (current->next) {
        if (time < current->next->time) {   // add in-between nodes
            dt->next = current->next;
            current->next = dt;
            UNLOCK;
            return;
        }
        current = current->next;
    }
    current->next = dt;     // add to end
    UNLOCK;
}

Task* TaskQueue::getTask() {
    u_int64_t now = getCurrentTime();
    LOCK;
    u_int64_t interval = now - lastNow;
    lastNow = now;
    if (interval > maxInterval) maxInterval = interval;
    if (delayedTask && now >= delayedTask->time) {
        DelayedTask* current = delayedTask;
        delayedTask = delayedTask->next;
        Task* task = current->task;
        ASSERT_NULL(task->next);
        delete current;
        UNLOCK;
        return task;     // for new immediately run the task
    }
    Task* result = 0;
    if (firstLow) {
        result = firstLow;
        firstLow = firstLow->next;
        result->next = 0;
    }
    UNLOCK;
    return result;
}

