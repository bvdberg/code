#include <stdio.h>
#include <string>
#include <deque>

class MudPlayer {};
class MudObject {};

class MudRoom {
public:
    MudRoom(const std::string& name_, const std::string& descr);
    ~MudRoom();

    //virtual void bla() {};    // 4 bytes 
private:
//    std::string name;         // 4 bytes
//    std::string description;  // 4 bytes
//    int exits[4];             // 16 bytes
//    bool test;                  // 4 bytes (contains 4 bools)
    typedef std::deque<MudPlayer*> Players;
    Players players;            // 40 bytes
};


int main() {
    printf("sizeof(MudRoom) = %u\n", sizeof(MudRoom));
    return 0;
}



