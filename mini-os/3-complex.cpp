#include <stdio.h>

// ----------------------------------------------------------------
// FRAMEWORK
// TODO progress function

typedef void (*InitFun)(int taskid);
typedef int (*ProcessFun)(int events);
typedef void (*CancelFun)(void);

struct Task {
    Task* next;
    InitFun init;
    ProcessFun process;
    CancelFun cancel;
    int id;
    int events;

    Task(InitFun init_, ProcessFun process_, CancelFun cancel_, int id_)
        : next(0)
        , init(init_)
        , process(process_)
        , cancel(cancel_)
        , id(id_)
        , events(0)
    {}
};

Task* task_head = 0;
Task* task_tail = 0;

void addTask(InitFun init_, ProcessFun process_, CancelFun cancel_) {
    if (task_tail == 0) {
        task_head = new Task(init_, process_, cancel_, 1);
        task_tail = task_head;
        return;
    }
    task_tail->next = new Task(init_, process_, cancel_, task_tail->id + 1);
    task_tail = task_tail->next;
}

// ----------------------------------------------------------------
// TASK1

static int task1_id = 0;
static int task1_cancelled = 0;

void task1Init(int taskid) {
    task1_id = taskid;
    printf("%s (taskid=%d)\n", __func__, task1_id);
}

int task1Processor(int events) {
    printf("%s (events=%d, taskid=%d)\n", __func__, events, task1_id);
    if (task1_cancelled) {
        printf("task1 cancelled\n");
        return 0;
    }
    // task takes <events> number of  processes
    return events-1;
}

void task1Cancel() {
    printf("%s (taskid=%d)\n", __func__, task1_id);
    task1_cancelled = 1;

}

// ----------------------------------------------------------------
// TASK2

static int task2_id = 0;
static int task2_cancelled = 0;

void task2Init(int taskid) {
    task2_id = taskid;
    printf("%s (taskid=%d)\n", __func__, task2_id);
}

int task2Processor(int events) {
    printf("%s (events=%d, taskid=%d)\n", __func__, events, task2_id);
    return 0;
}

void task2Cancel() {
    printf("%s (taskid=%d)\n", __func__, task2_id);

}

// ----------------------------------------------------------------
// MAIN

void cancelTask(int id) {
    for (Task* task=task_head; task!=0; task = task->next) {
        if (task->id == id) task->cancel();
    }
    // ERROR
}

void interrupt() {
    printf(">> interrupt <<\n");
    // always cancel task1, normally find Task + cancel
    cancelTask(1);

}

int main() {
    // register tasks
    addTask(task1Init, task1Processor, task1Cancel);
    addTask(task2Init, task2Processor, task2Cancel);

    // run init
    for (Task* task=task_head; task!=0; task = task->next) {
        task->init(task->id);
    }

    // main loop
    int tick = 0;
    while (tick < 100) {
        for (Task* task=task_head; task!=0; task = task->next) {
            if (task->events) {
                task->events = task->process(task->events);
            }
            // MANUALLY INSERT EVENTS
            if (task->id==1 && tick==10) task->events = 30;
            if (task->id==1 && tick==20) interrupt();
            if (task->id==2 && tick==15) task->events = 152;
        }
        tick++;
    }
    

    return 0;
}

