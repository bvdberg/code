#include <deque>
#include <vector>
#include <list>
#include <algorithm>

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

using namespace std;

u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
}


class Object {};

class Error {};

class Map {
public:
    ~Map() { while (!items.empty()) { delete items.back(); items.pop_back(); } }

    typedef std::list<Object*> Items;
    typedef Items::const_iterator ItemsConstIter;
    typedef Items::iterator ItemsIter;

    void addItem(Object* item) {
        items.push_back(item);
    }
    void removeItem(const Object* item) {
        ItemsIter iter = find(items.begin(), items.end(), item);
        if (iter == items.end()) throw Error();
        items.erase(iter);
    }
    const Items getCopy() const { return items; }
private:
    Items items;
    Map& operator=(const Map&);
};


class Deque {
public:
    ~Deque() { while (!items.empty()) { delete items.back(); items.pop_back(); } }

    typedef std::deque<Object*> Items;
    typedef Items::const_iterator ItemsConstIter;
    typedef Items::iterator ItemsIter;

    void addItem(Object* item) {
        items.push_back(item);
    }
    void removeItem(const Object* item) {
        ItemsIter iter = find(items.begin(), items.end(), item);
        if (iter == items.end()) throw Error();
        items.erase(iter);
    }
    const Items getCopy() const { return items; }
private:
    Items items;
    Deque& operator=(const Deque&);
};


class Vector {
public:
    ~Vector() { while (!items.empty()) { delete items.back(); items.pop_back(); } }

    typedef std::vector<Object*> Items;
    typedef Items::const_iterator ItemsConstIter;
    typedef Items::iterator ItemsIter;

    void addItem(Object* item) {
        items.push_back(item);
    }
    void removeItem(const Object* item) {
        ItemsIter iter = find(items.begin(), items.end(), item);
        if (iter == items.end()) throw Error();
        items.erase(iter);
    }
    const Items getCopy() const { return items; }
private:
    Items items;
    Vector& operator=(const Vector&);
};


int main(int argc, char* argv[]) {
    int count = atoi(argv[1]) * 100;
//    Map container; printf("map %d ", count);
    Deque container; printf("deque %d ", count);
//    Vector container; printf("vector %d ", count);
    for (int i=0; i<count; ++i) {
        container.addItem(new Object());
    }

    u_int64_t begin = getCurrentTime();
//    const Map::Items copy = container.getCopy();
    const Deque::Items copy = container.getCopy();
//    const Vector::Items copy = container.getCopy();
    u_int64_t end = getCurrentTime();
    u_int64_t diff = end - begin;
    printf("%llu usec\n", diff);
    return 0;
}

