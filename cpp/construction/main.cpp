#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

class Base {
public:
    Base() {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
    virtual void run() { printf("  %s\n", __PRETTY_FUNCTION__); }
};

class Derived : public Base {
public:
    Derived() {
        printf("%s\n", __PRETTY_FUNCTION__);
        pthread_create(&thread, 0, threadMain, (void*) this);
        sleep(2);
    }
    static void* threadMain(void* ptr) {
        Base* b = reinterpret_cast<Base*>(ptr);
        for (int i=0; i<4; i++) {
            printf("in thread: ");
            b->run();
            sleep(1);
        }
    }
    virtual void run() { printf("  %s\n", __PRETTY_FUNCTION__); }
    pthread_t thread;
};

class DerivedDerived : public Derived {
public:
    DerivedDerived() {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
    virtual void run() { printf("  %s\n", __PRETTY_FUNCTION__); }
};

int main() {
    DerivedDerived dd;
    sleep(4);
    return 0;
}



