#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdint.h>

class MemBlock;

#define BLOCK_SIZE 64*1024

class Context {
public:
    Context(uint32_t blockSize_ = BLOCK_SIZE);
    ~Context();

    void* Allocate(size_t Size, unsigned Align = 8) const;
    void Deallocate(void* Ptr) const {}

    void dump() const;
    size_t getAllocatedMemory() const;
private:
    void allocateMemory() const;

    mutable MemBlock* blocks;
    mutable MemBlock* lastBlock;
    uint32_t blockSize;

    Context(const Context&);
    Context& operator= (const Context&);
};

// TODO add operator new[] and delete[]

inline void *operator new(size_t Bytes, const Context &C, size_t Alignment) {
    return C.Allocate(Bytes, Alignment);
}

// TODO never seems to be used
inline void operator delete(void *Ptr, const Context &C, size_t) {
    C.Deallocate(Ptr);
}


#endif

