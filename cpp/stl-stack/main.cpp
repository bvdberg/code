#include <stdio.h>
#include <stack>

using namespace std;

int main() {
    stack<int> s;
    s.push(8);
    s.push(4);
    s.push(3);
    printf("top = %d\n", s.top());

    s.pop();
    printf("top = %d\n", s.top());

    while (!s.empty()) {
        s.pop();
    }

    return 0;
}

