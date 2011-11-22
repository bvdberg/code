#include <stdio.h>
#include <stdlib.h>

#include "circbuf.h"

unsigned int buffer_data(Buffer* buf) {
    if (buf->putIndex == buf->takeIndex) return 0;
    if (buf->putIndex > buf->takeIndex) { // no wraparound
        return buf->putIndex - buf->takeIndex;
    } else { // wraparound
         return buf->takeIndex - buf->putIndex - 1;
    }
}


unsigned int buffer_free(Buffer* buf) {
    if (buf->putIndex == buf->takeIndex) {  // empty buffer
        return buf->size - 1;
    }
    return buf->size - 1 - buffer_data(buf);
}


void buffer_add(Buffer* buf, int amount) {
    if (buffer_free(buf) < amount) {
        printf("ERROR not enough space\n");
        exit(-1);
    }
    int beforeWrap = buf->size - buf->putIndex;
    if (beforeWrap >= amount) { // no wraparound
        buf->putIndex += amount;
    } else {
        buf->putIndex += amount; 
        buf->putIndex = buf->putIndex % buf->size;
    }
}


void buffer_remove(Buffer* buf, int amount) {
    if (buffer_data(buf) < amount) {
        printf("ERROR not enough data\n");
        exit(-1);
    }
    int offset = buf->putIndex - buf->takeIndex;
    if (offset > 0) { // no wraparound
        buf->takeIndex += amount;
    } else {
        buf->takeIndex += amount;
        buf->takeIndex = buf->takeIndex % buf->size;
    }
}


void buffer_print(Buffer* buf) {
    int i;
    printf("P=%2d  T=%2d  F=%2d: ", buf->putIndex, buf->takeIndex, buffer_free(buf));
    printf("Buffer [");
    if (buf->putIndex >= buf->takeIndex) {   // no wraparound
        for (i=0; i<buf->size; i++) {
            if (i < buf->takeIndex) printf("_");
            else if (i >= buf->putIndex) printf("_");
            else printf("D");
            if (i%5 == 4 && i!=buf->size-1) printf(".");
        }
    } else {
        for (i=0; i<buf->size; i++) {
            if (i < buf->putIndex) printf("D");
            else if (i < buf->takeIndex) printf("_");
            else printf("D");
            if (i%5 == 4 && i!=buf->size-1) printf(".");
        }
    }
    printf("]\n");
}

