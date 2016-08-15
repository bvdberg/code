#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdint.h>

#include <stdio.h>  // for printf

class Context {
public:
    Context();
    ~Context();

    void* Allocate(size_t Size, unsigned Align = 8) const {
        // TODO use Align
        printf("Allocate %lu / %lu\n", Size, sizeof(pool) - (pointer-pool));
        uint8_t* result = pointer;
        pointer += Size;
        return pointer;
    }
    void Deallocate(void* Ptr) const {}
private:
    uint8_t pool[4096];
    mutable uint8_t* pointer;

    Context(const Context&);
    Context& operator= (const Context&);
};

// TODO add operator new[] and delete[]

inline void *operator new(size_t Bytes, const Context &C, size_t Alignment) {
    return C.Allocate(Bytes, Alignment);
}

inline void operator delete(void *Ptr, const Context &C, size_t) {
    C.Deallocate(Ptr);
}


#endif

