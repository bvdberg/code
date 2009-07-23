#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>

namespace Mud {

template <typename T, size_t maxBlocks>
class Cache {
public:
    Cache() : count(0) {}
    ~Cache() {
        while (count != 0) {
            --count;
            ::operator delete(blocks[count]);
        }
    }
    void* get() {
        if (count == 0) {
            return ::operator new(sizeof(T));
        }
        --count;
        return blocks[count];
    }
    void put(void* block) {
        if (count < maxBlocks) {
            blocks[count] = block;
            ++count;
        } else {
            ::operator delete(block);
        }
    }
private:
    void* blocks[maxBlocks];
    size_t count;
};

}

#endif

