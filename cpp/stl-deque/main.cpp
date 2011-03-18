#include <stdio.h>
#include <deque>
#include <algorithm>

using namespace std;

class Container {
public:
    Container() {
        printf("size of deque=%d\n", sizeof(numbers));
    }
    void add(int value) {
        numbers.push_back(value);
        printf("adding %d size=%d\n", value, numbers.size());
    }
    void addOrdered(int value) {
        NumbersIter iter = numbers.begin();
        while (iter != numbers.end()) {
            int num = *iter;
            if (num > value) break;
            ++iter;
        }
        numbers.insert(iter, value);
        printf("adding ordered %d size=%d\n", value, numbers.size());
    }
    void remove(int value) {
        NumbersIter iter = find(numbers.begin(), numbers.end(), value);
        if (iter == numbers.end()) {
            printf("NO SUCH NUMBER\n");
        } else {
            numbers.erase(iter);
        }
    }
    void test() {
        print();

        NumbersIter iter = numbers.begin();
        while (iter != numbers.end()) {
            printf("at element: %d\n", *iter);
            if (*iter == 10) {
                printf("deleting %d\n", *iter);
                iter = numbers.erase(iter);
            } else {
                iter++;
            }
        }

        print();
    }
    void print() const {
        printf("Deque:\n");
        for (NumbersConstIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("  %d \n", *iter);
        }
    }
    void printRev() const {
        printf("Deque: (rev)\n");
        for (NumbersConstRevIter iter = numbers.rbegin(); iter != numbers.rend(); ++iter) {
            printf("  %d \n", *iter);
        }
    }
private:
    typedef std::deque<int> Numbers;
    typedef Numbers::iterator NumbersIter;
    typedef Numbers::const_iterator NumbersConstIter;
    typedef Numbers::const_reverse_iterator NumbersConstRevIter;
    Numbers numbers;
};

int main() {
    Container con;
    con.add(5);
    con.add(7);
    con.add(10);
    con.print();
    con.printRev();

    con.remove(7);
    con.remove(12);

    con.test();

    Container con2;
    con2.addOrdered(7);
    con2.addOrdered(1);
    con2.addOrdered(9);
    con2.addOrdered(8);
    con2.addOrdered(4);
    con2.addOrdered(2);
    con2.print();

    return 0;
}

