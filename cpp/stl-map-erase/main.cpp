#include <stdio.h>
#include <string>
#include <map>

using namespace std;

class ClassA {
public:
    ClassA(const string& n) : name(n) {}
    string name;
};

typedef std::map<std::string, ClassA*> Channels;
typedef Channels::iterator ChannelsIter;

int main() {
    Channels channels;
    channels["a"] = new ClassA("A");
    channels["f"] = new ClassA("F");
    channels["c"] = new ClassA("C");
    channels["d"] = new ClassA("D");
    channels["e"] = new ClassA("E");
    channels["b"] = new ClassA("B");

    printf("removing 'c':\n");

    ChannelsIter iter = channels.begin();
    while (iter != channels.end()) {
        if (iter->first == "a") {
            printf("removing %s\n", iter->first.c_str());
            delete iter->second;
            channels.erase(iter++);
        } else {
            printf("%s -> %s\n", iter->first.c_str(), iter->second->name.c_str());
            ++iter;
        }
    }


    return 0;
}

