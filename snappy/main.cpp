#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#include "snappy.h"
#include "FileMap.h"

using namespace Test;
using namespace snappy;

static uint64_t getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        return -1;
    }
    const char* infile = argv[1];

    FileMap input(infile);
    input.open();

    void* indata = input.region;
    unsigned insize = input.size;

    const unsigned runs = 4;

    for (unsigned i=0; i<runs; i++) {
        uint64_t t1 = getCurrentTime();
        std::string outdata;
        size_t outsize = snappy::Compress((const char*)indata, insize, &outdata);
        uint64_t t2 = getCurrentTime();
        int percentage = (outsize*100)/insize;
        printf("Compress: %u -> %lu bytes (%d%%), took %lu ms\n", insize, outsize, percentage, (t2 - t1)/1000);
    }

    for (unsigned i=0; i<runs; i++) {
        uint64_t t1 = getCurrentTime();
        size_t outsize = 0;
        // just use same size as input for now
        char* compressed = (char*)malloc(insize);
        snappy::RawCompress((const char*)indata, insize, compressed, &outsize);
        int percentage = (outsize*100)/insize;
        uint64_t t2 = getCurrentTime();
        printf("RawCompress: %u -> %lu bytes (%d%%), took %lu ms\n", insize, outsize, percentage, (t2 - t1)/1000);
        free(compressed);
    }

    return 0;
}

