#include <stdio.h>
#include <map>
#include <stdint.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


using namespace std;

class Container {
public:
    void add(int key, int value) {
        numbers[key] = value;
        printf("adding %2d -> %2d  size=%d\n", key, value, numbers.size());
    }
    void remove(int value) {
        NumbersIter iter = numbers.find(value);
        if (iter == numbers.end()) {
            printf("%s() NO SUCH ELEMENT (%d)\n", __func__, value);
        } else {
            numbers.erase(iter);
        }
    }
    void speedTest() {
        numbers.clear();
        for (int i=1000; i<2000; i++) {
            numbers[i] = i*10;
        }
        uint64_t one = rdtsc();
        NumbersConstIter iter = numbers.begin();
        int first = iter->first;
        uint64_t two = rdtsc();
        printf("first=%d  diff = %llu\n", first, two - one);

        one = rdtsc();
        NumbersConstIter iter2 = numbers.find(1000);
        first = iter2->first;
        two = rdtsc();
        printf("first=%d  diff = %llu\n", first, two - one);
    }


    void print() {
        printf("--map--\n");
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("  %2d -> %2d\n", iter->first, iter->second);
        }
    }
private:
    typedef std::map<int, int> Numbers;
    typedef Numbers::iterator NumbersIter;
    typedef Numbers::const_iterator NumbersConstIter;
    Numbers numbers;
};


int main() {
    Container con;
    con.add(5, 10);
    con.add(7, 14);
    con.add(10, 20);

//    con.print();

    con.speedTest();

    return 0;
}

