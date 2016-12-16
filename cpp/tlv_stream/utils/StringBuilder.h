#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <string>
#include <sys/types.h>
#include <inttypes.h>

#define PRINTF_FORMAT_CHECK(format_index, args_index) __attribute__ ((__format__(printf, format_index, args_index)))

class StringBuilder {
public:
    StringBuilder(unsigned cap = CAPACITY, char* buf = 0);
    ~StringBuilder();

    StringBuilder& operator<<(const char* input);
    StringBuilder& operator<<(void* input);
    StringBuilder& operator<<(const std::string& input);
    StringBuilder& operator<<(char input);
    StringBuilder& operator<<(int32_t input);
    StringBuilder& operator<<(uint32_t input);
    StringBuilder& operator<<(int64_t input);
    StringBuilder& operator<<(uint64_t input);
    StringBuilder& operator<<(const StringBuilder& input);

    void print(const char* format, ...) PRINTF_FORMAT_CHECK(2, 3);
    void number(unsigned radix, int64_t value);

    operator const char*() const { return buffer; }
    const char* c_str() const { return buffer; }
    void clear();
    unsigned size() const { return (unsigned)(ptr - buffer); }
    bool isEmpty() const { return (size() == 0); }
    unsigned getCapacity() const { return capacity; }

    void strip(char c);
    void indent(unsigned num);
    void setColor(const char* color);
    void enableColor(bool enable) { colors = enable; }
private:
    unsigned space_left() const { return capacity - size(); }

    static const unsigned CAPACITY = 1024*1024;
    unsigned capacity;
    char* buffer;
    char* ptr;
    bool ownBuf;
    bool colors;

    StringBuilder(const StringBuilder&);
    StringBuilder& operator= (const StringBuilder&);
};

#endif

