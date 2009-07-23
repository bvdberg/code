#include <vector>

using namespace std;

template <typename T>
class MyClass {
public:
    typedef std::vector<T> Container;
    typedef typename std::vector<T>::iterator ContainerIter;
//    typedef std::vector<T>::iterator ContainerIter;   // doesn't work
    Container container;

};


int main() {
    MyClass<int> a;
    
    return 0;
}
