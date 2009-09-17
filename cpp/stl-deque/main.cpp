#include <stdio.h>
#include <deque>
#include <algorithm>

using namespace std;

class Container {
public:
    void add(int value) {
        numbers.push_back(value);
        printf("adding %d size=%d\n", value, numbers.size());
    }
    void remove(int value) {
        NumbersIter iter = find(numbers.begin(), numbers.end(), value);
        if (iter == numbers.end()) {
            printf("NO SUCH NUMBER\n");
        } else {
            numbers.erase(iter);
        }
    }
    void print() {
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("Contains: %d \n", *iter);
        }

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

        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("Contains: %d\n", *iter);
        }
    }
private:
    typedef std::deque<int> Numbers;
    typedef Numbers::iterator NumbersIter;
    Numbers numbers;
};

int main() {
    Container con;
    con.add(5);
    con.add(7);
    con.add(10);

    con.remove(7);
    con.remove(12);

    con.print();

    return 0;
}

