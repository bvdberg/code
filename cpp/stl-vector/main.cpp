#include <stdio.h>

#include <vector>

using namespace std;

int main() {

    typedef std::vector<int> Vector;
    typedef Vector::reverse_iterator VectorRevIter;
    Vector v;

    printf("size=%d, capacity=%d\n", v.size(), v.capacity());
    v.resize(5);  //put 10 elements in
    printf("size=%d, capacity=%d\n", v.size(), v.capacity());
    vector<int>::iterator iter;
    int i = 0;
    for(iter=v.begin(); iter!=v.end(); iter++) {
        printf("%d -> %d\n", i++, (*iter));
    }
    v.reserve(10);
    printf("size=%d, capacity=%d\n", v.size(), v.capacity());
    i = 0;
    for(iter=v.begin(); iter!=v.end(); iter++) {
        printf("%d -> %d\n", i++, (*iter));
    }

    for(i=0; i<10; i++) v.push_back(i);
    printf("size=%d, capacity=%d\n", v.size(), v.capacity());
    i = 0;
    for(iter=v.begin(); iter!=v.end(); iter++) {
        printf("%d -> %d\n", i++, (*iter));
    }
    
    printf("reverse\n");
    for (VectorRevIter iter = v.rbegin(); iter != v.rend(); ++iter) {
        printf("%d\n", *iter);
    }

    return 0;
}

