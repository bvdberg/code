#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

class Context;

//#define NOEXCEPT noexcept
#define NOEXCEPT throw()

class Base {
public:
    Base(int id_);
    virtual ~Base();
protected:
    void* operator new(size_t bytes) {
        assert(0 && "Base cannot be allocated with regular 'new'");
    }
    void operator delete(void* data) {
        assert(0 && "Base cannot be released with regular 'delete'");
    }
public:
    // Clang: avoid using NOEXCEPT, because this will cause generation
    // of a null check on the result.
    // Dont use delete on the pointer, since all memory will be freed when
    // context is deleted.
    void* operator new(size_t bytes, const Context& C);
    void* operator new(size_t bytes, const Context* C) {
        return operator new(bytes, *C);
    }
    // placement operator?
    // void* operator new(size_t bytes, void* mem) NOEXCEPT { return mem; }

    void operator delete(void*, const Context& C) NOEXCEPT {}
    void operator delete(void*, const Context* C) NOEXCEPT {}
    // Q: why these?)
    void operator delete(void*, size_t) NOEXCEPT {}
    void operator delete(void*, void*) NOEXCEPT {}
private:
    int id;
    uint8_t data[10];

    Base(const Base&);
    Base& operator= (const Base&);
};

#endif

