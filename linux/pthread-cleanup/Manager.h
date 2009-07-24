#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>

struct Param {
    int id;
    bool stop;
    Param(int n): id(n), stop(false) {}
};

class Manager
{

public:
    Manager();
    virtual ~Manager();
    void start();
    void stop();
    void cancel();

private:
    Manager(const Manager& toBeCopied);
    Manager& operator=(const Manager& toBeCopied);

    pthread_t threads[2];
    Param* params[2];
};

#endif // MANAGER_H

