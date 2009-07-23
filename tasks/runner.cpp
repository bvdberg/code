#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <sched.h>
#include <unistd.h>

#include "Cache.h"
#include "color.h"
#include "TaskQueue.h"

TaskQueue* taskQueue = 0;


class Runnable {
public:
    Runnable() : stopLoop(false),  thread1(0) {}
    virtual ~Runnable() {}
    void start() {
        pthread_create(&thread1, 0, Runnable::threadMain, (void*) this);
    }
    void stop() {
        if (stopLoop) return;
        stopLoop = true;
        if (thread1) pthread_join(thread1, 0);
    }
protected:
    volatile bool stopLoop;
    virtual void run() = 0;
private:
    static void* threadMain(void* runner_ptr) {
        Runnable* runner = reinterpret_cast<Runnable*>(runner_ptr);
        assert(runner);
        runner->run();
        return 0;
    }

    pthread_t thread1;
};


class TaskRunner : public Runnable {
public:
    TaskRunner(char* name_, char* color_) : name(name_), color(color_) {}
    virtual ~TaskRunner() { stop(); }
    void run() {
        while (!stopLoop) {
            Task* task = taskQueue->getTask();
            if (!task) {
                usleep(10000);
//                pthread_yield();
                continue;
            }

            while (task && !stopLoop) {
                bool reschedule = task->run(color);
                Task* nextTask = task->next;

                if (reschedule) { task->next = 0; taskQueue->addTask(task); }
                else delete task;

                task = nextTask;
            }
            if (task) taskQueue->addTask(task); // put back in queue
        }
    }
private:
    char* name;
    char* color;
};


class ReadLockerTask : public Task {
public:
    ReadLockerTask(MultipleReaderLock* lock_, Task* task_)
        : Task("Locker")
        , lock(lock_)
        , task(task_)
    {}
    ~ReadLockerTask() {
        if (task) delete task;
    }
    virtual bool run(char*) {
        lock->reader_lock(this, task);
        task = 0;
        return false;
    }
private:
    MultipleReaderLock* lock;
    Task* task;
};


class WriteLockerTask : public Task {
public:
    WriteLockerTask(MultipleReaderLock* lock_, Task* task_)
        : Task("Locker")
        , lock(lock_)
        , task(task_)
    {}
    ~WriteLockerTask() {
        if (task) delete task;
    }
    virtual bool run(char*) {
        lock->writer_lock(this, task);
        task = 0;
        return false;
    }
private:
    MultipleReaderLock* lock;
    Task* task;
};


class LockerTask : public Task {
public:
    LockerTask(TaskLock* lock_, Task* task_)
        : Task("Locker")
        , lock(lock_)
        , task(task_)
    {}
    ~LockerTask() {
        if (task) delete task;
    }
    virtual bool run(char*) {
        lock->lock(this, task);
        task = 0;
        return false;
    }
private:
    TaskLock* lock;
    Task* task;
};

// --------------------------------------------------------

TaskLock lock1;
TaskLock lock2;
TaskLock lock3;
TaskLock lock4;
TaskLock lock5;
TaskLock lock6;

MultipleReaderLock rlock1;

struct Counter {
    Counter() : readCount(0), writeCount(0) {}
    int readCount;
    int writeCount;
};

Counter counter1;
Counter counter2;
Counter counter3;
Counter counter4;
Counter counter5;
Counter counter6;


class Reader : public Task {
public:
    Reader(char* name_, Counter* counter_, TaskLock* lock_) : Task(name_), lock(lock_), rlock(0), counter(counter_) {}
    Reader(char* name_, Counter* counter_, MultipleReaderLock* rlock_) : Task(name_), lock(0), rlock(rlock_), counter(counter_) {}
    virtual bool run(char* color) {
        printf("%s%s  %d / %d\n", color, name, counter->readCount, counter->writeCount);
        counter->readCount++;
        usleep(40000);
        printf("%s%s done\n", color, name);
        if (lock) {
            lock->unlock();
            taskQueue->addDelayedTask(getCurrentTime() + 250000, new LockerTask(lock, new Reader(name, counter, lock)));
        }
        if (rlock) {
            rlock->reader_unlock();
            taskQueue->addDelayedTask(getCurrentTime() + 10000, new ReadLockerTask(rlock, new Reader(name, counter, rlock)));
        }
        return false;
    }
private:
    TaskLock* lock;
    MultipleReaderLock* rlock;
    Counter* counter;
};


class Writer : public Task {
public:
    Writer(char* name_, Counter* counter_, TaskLock* lock_) : Task(name_), lock(lock_), rlock(0), counter(counter_) {}
    Writer(char* name_, Counter* counter_, MultipleReaderLock* rlock_) : Task(name_), lock(0), rlock(rlock_), counter(counter_) {}
    virtual bool run(char* color) {
        printf("%s%s  %d / %d\n", color, name, counter->readCount, counter->writeCount);
        usleep(42000);
        counter->writeCount++;
        printf("%s%s done\n", color, name);
        // weak: writer shouldn't have to know if locked
        if (lock) {
            lock->unlock();
            taskQueue->addDelayedTask(getCurrentTime() + 250000, new LockerTask(lock, new Writer(name, counter, lock)));
        }
        if (rlock) {
            rlock->writer_unlock();
            taskQueue->addDelayedTask(getCurrentTime() + 20000, new WriteLockerTask(rlock, new Writer(name, counter, rlock)));
        }
        return false;
    }
private:
    TaskLock* lock;
    MultipleReaderLock* rlock;
    Counter* counter;
};


class PeriodicTask : public Task {
public:
    PeriodicTask(u_int64_t time_) : Task("Periodic"), time(time_) {}
    virtual bool run(char* color) {
        u_int64_t now = getCurrentTime();
        printf("%s%s running at %llu (%llu too late)\n", color, name, now, now - time);
        taskQueue->addDelayedTask(time + 1000000, new PeriodicTask(time + 1000000));
        
        return false;
    }
private:
    u_int64_t time;
};


int main() {
    taskQueue = new TaskQueue();
    TaskRunner runner1("thread1", ANSI_RED);
    TaskRunner runner2("thread2", ANSI_GREEN);
    TaskRunner runner3("thread3", ANSI_YELLOW);
    TaskRunner runner4("thread4", ANSI_BLUE);
    TaskRunner runner5("thread5", ANSI_BROWN);
    runner1.start();
    runner2.start();
    runner3.start();
    runner4.start();
    runner5.start();

    taskQueue->addTask(new ReadLockerTask(&rlock1, new Reader("reader1a", &counter1, &rlock1)));
    taskQueue->addTask(new ReadLockerTask(&rlock1, new Reader("reader1b", &counter1, &rlock1)));
    taskQueue->addTask(new ReadLockerTask(&rlock1, new Reader("reader1c", &counter1, &rlock1)));
    taskQueue->addTask(new ReadLockerTask(&rlock1, new Reader("reader1d", &counter1, &rlock1)));
    taskQueue->addTask(new WriteLockerTask(&rlock1, new Writer("writer1", &counter1, &rlock1)));

/*
    taskQueue->addTask(new PeriodicTask(getCurrentTime()));

    taskQueue->addTask(new LockerTask(&lock2, new Reader("reader2", &counter2, &lock2)));
    taskQueue->addTask(new LockerTask(&lock2, new Writer("writer2", &counter2, &lock2)));

    taskQueue->addTask(new LockerTask(&lock3, new Reader("reader3", &counter3, &lock3)));
    taskQueue->addTask(new LockerTask(&lock3, new Writer("writer3", &counter3, &lock3)));

    taskQueue->addTask(new LockerTask(&lock4, new Reader("reader4", &counter4, &lock4)));
    taskQueue->addTask(new LockerTask(&lock4, new Writer("writer4", &counter4, &lock4)));

    taskQueue->addTask(new LockerTask(&lock5, new Reader("reader5", &counter5, &lock5)));
    taskQueue->addTask(new LockerTask(&lock5, new Writer("writer5", &counter5, &lock5)));

    taskQueue->addTask(new LockerTask(&lock6, new Reader("reader6", &counter6, &lock6)));
    taskQueue->addTask(new LockerTask(&lock6, new Writer("writer6", &counter6, &lock6)));
*/
    sleep(10);

    runner5.stop();
    runner4.stop();
    runner3.stop();
    runner2.stop();
    runner1.stop();

    printf(ANSI_NORMAL);
    printf("Max interval = %llu\n", taskQueue->maxInterval);
    printf("Counter1:  %4d / %4d\n", counter1.readCount, counter1.writeCount);
    printf("Counter2:  %4d / %4d\n", counter2.readCount, counter2.writeCount);
    printf("Counter3:  %4d / %4d\n", counter3.readCount, counter3.writeCount);
    printf("Counter4:  %4d / %4d\n", counter4.readCount, counter4.writeCount);
    printf("Counter5:  %4d / %4d\n", counter5.readCount, counter5.writeCount);
    printf("Counter6:  %4d / %4d\n", counter6.readCount, counter6.writeCount);

    delete taskQueue;
    return 0;
}

