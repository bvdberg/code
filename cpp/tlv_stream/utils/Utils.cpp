#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#include "utils/Utils.h"
#include "utils/StringBuilder.h"

static uint64_t start_time;

uint64_t Utils::getCurrentTime()
{
#if 0
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec/1000);
    return now64;
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    uint64_t tv64 = tv.tv_sec;
    tv64 *= 1000000;
    tv64 += tv.tv_usec;
    return tv64;
#endif
}

uint64_t Utils::getTime()
{
    uint64_t now = getCurrentTime();
    now -= start_time;
    return now;
}

void Utils::setStartTime() {
    start_time = getCurrentTime();
}

uint64_t Utils::getStartTime() {
    return start_time;
}

void Utils::printTime(StringBuilder& output, const uint64_t now) {
    uint64_t subsec = now % 1000000lu;
    output.print("%04" PRIu64, now / 1000000lu);
    output.print(".%06" PRIu64, subsec);
}

const char* Utils::timeString() {
    static char result[32];
    char* cp = result;
    uint64_t now = getTime();
    uint64_t subsec = now % 1000000lu;
    cp += sprintf(cp, "%04" PRIu64, now / 1000000lu);
    sprintf(cp, ".%06" PRIu64, subsec);
    return result;
}

const char* Utils::printDeci(uint32_t number) {
    static char buffer[4][24];
    static uint8_t index = 0;
    index++;
    if (index == 4) index = 0;
    char* cp = buffer[index];
    const uint32_t whole = number / 10;
    const uint32_t decimal = number % 10;
    if (decimal == 0) {
        sprintf(cp, "%u", whole);
    } else {
        sprintf(cp, "%u.%u", whole, decimal);
    }
    return cp;
}

const char* Utils::printBinary(const uint8_t value) {
    static char buffer[9];
    sprintf(buffer, "%d%d%d%d",
        (value & (1<<3)) != 0,
        (value & (1<<2)) != 0,
        (value & (1<<1)) != 0,
        (value & (1<<0)) != 0);
    return buffer;
}

void Utils::dumpHex(const uint8_t* data, unsigned size) {
    StringBuilder out;
    for (unsigned i=0; i<size; i++) {
        out.print(" %02x", data[i]);
    }
    printf("(%d) %s\n", size, out.c_str());
}

void Utils::dumpHex(StringBuilder& out, const char* text, const uint8_t* data, unsigned size) {
    out << text << " (" << size << ")";
    for (unsigned i=0; i<size; i++) out.print(" %02x", data[i]);
}

void Utils::initRand() {
    srand(getCurrentTime());
}

unsigned Utils::getRandom(unsigned max) {
    unsigned value = rand() % (max + 1);
    return value;
}

