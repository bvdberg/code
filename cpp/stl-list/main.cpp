#include <list>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdint.h>

using namespace std;

class Container {
public:
    Container() {
        printf("size of list=%d\n", sizeof(numbers));
    }
    void add(int value) {
        numbers.push_back(value);
        printf("adding %d size=%d\n", value, numbers.size());
    }
    void addOrdered(int value) {
        NumbersIter iter;
        for (iter=numbers.begin(); iter != numbers.end(); iter++) {
            if ((*iter) >= value) break;
        }
        if (iter == numbers.end()) numbers.push_back(value);
        else numbers.insert(iter, value);
        printf("adding %d size=%d\n", value, numbers.size());
    }
    void remove(int value) {
        NumbersIter iter = find(numbers.begin(), numbers.end(), value);
        if (iter == numbers.end()) {
            printf("%s() NO SUCH NUMBER (%d)\n", __func__, value);
        } else {
            numbers.erase(iter);
        }
    }
    void moveToFront(int value) {
        NumbersIter iter = find(numbers.begin(), numbers.end(), value);
        if (iter == numbers.end()) {
            printf("%s() NO SUCH NUMBER (%d)\n", __func__, value);
        } else {
            printf("%s() moving %d to front\n", __func__, value);
            numbers.erase(iter);
            numbers.push_front(*iter);
        }
    }
    void test() {
        printf("%s()\n", __func__);
        //iterates over all items and moves them all to front
        NumbersIter iter = numbers.begin();
        while (iter != numbers.end()) {
            int value = *iter;
            printf("  %3d\n", value);
            iter = numbers.erase(iter);
            numbers.push_front(value);
        }
    }
    void print() {
        printf("--list--\n");
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf(" %3d\n", *iter);
        }
    }
private:
    typedef std::list<int> Numbers;
    typedef Numbers::iterator NumbersIter;
    Numbers numbers;
};

int main() {
    Container con;
    con.add(5);
    con.add(10);
    con.addOrdered(7);
    con.addOrdered(7);
    con.add(15);

    con.remove(7);
    con.remove(12);

    con.print();

    con.moveToFront(15);
    con.print();

    con.test();

    return 0;
}

