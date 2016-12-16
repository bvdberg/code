#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>
#include <string>

#define USEC(x) (1lu*x)
#define MSEC(x) (1000lu*x)
#define SEC(x) (1000000lu*x)

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define BOOL2STR(x) ((x) ? "1" : "0")

#define ASSIGN_BIT(w, pos, value) (w |= (value<<pos))
#define SET_BIT(w, pos) (w |= (1<<pos))
#define CLR_BIT(w, pos) (w &= ~(1<<pos))
#define GET_BIT(w, pos) ((w >> pos) & 1)

class StringBuilder;

class Utils {
public:
    static uint64_t getCurrentTime();
    static uint64_t getTime();     // relative from start
    static void setStartTime();
    static uint64_t getStartTime();
    static const char* timeString();
    static void printTime(StringBuilder& output, const uint64_t now);

    static const char* printBinary(const uint8_t value);
    static const char* printDeci(uint32_t number);

    static void dumpHex(const uint8_t* data, unsigned size);
    static void dumpHex(StringBuilder& out, const char* text, const uint8_t* data, unsigned size);

    static void initRand();
    static unsigned getRandom(unsigned max);
};

#endif

