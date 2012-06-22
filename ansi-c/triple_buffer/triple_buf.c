#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "triple_buf.h"

void buffer_init(Buffer* buf) {
    int i;
    for (i=0; i<BUF_SIZE; i++) buf->data[i] = EMPTY;
}

void buffer_print(Buffer* buf) {
    char output[32];
    char* cp = output;
    int i;
    *cp++ = '[';
    for (i=0; i<BUF_SIZE; i++) {
        char tmp = '_';
        switch (buf->data[i]) {
        case EMPTY:
            tmp = '_';
            break;
        case FULL:
            tmp = '+';
            break;
        case BUSY_READING:
            tmp = 'R';
            break;
        case BUSY_WRITING:
            tmp = 'W';
            break;
        }
        *cp++ = tmp;
    }
    *cp++ =']';
    *cp = 0;
    puts(output);
#if 0
    for (i=0; i<BUF_SIZE; i++) {
        switch (buf->data[i]) {
        case EMPTY:
            printf("buffer[%d] = empty\n", i);
            break;
        case FULL:
            printf("buffer[%d] = full\n", i);
            break;
        case BUSY_READING:
            printf("buffer[%d] = busy_reading\n", i);
            break;
        case BUSY_WRITING:
            printf("buffer[%d] = busy_writing\n", i);
            break;
        }
    }
#endif
}

int read_start(Buffer* buf) {
    // find slot with state 'FULL' otherwise return -1
    int i;
    for (i=0; i<BUF_SIZE; i++) {
        if (buf->data[i] == FULL) {
            buf->data[i] = BUSY_READING;
            return i;
        }
    }
    return -1;
}

void read_done(Buffer* buf) {
    // find slot with state 'BUSY_READING' and set state to EMPTY
    int i;
    for (i=0; i<BUF_SIZE; i++) {
        if (buf->data[i] == BUSY_READING) {
            buf->data[i] = EMPTY;
            return;
        }
    }
    // error
}

int write_start(Buffer* buf) {
    // find slot with state 'EMPTY' otherwise return -1
    int i;
    for (i=0; i<BUF_SIZE; i++) {
        if (buf->data[i] == EMPTY) {
            buf->data[i] = BUSY_WRITING;
            return i;
        }
    }
    // otherwise find old FULL buffer
    for (i=0; i<BUF_SIZE; i++) {
        if (buf->data[i] == FULL) {
            buf->data[i] = BUSY_WRITING;
            return i;
        }
    }
    return -1;
}

void write_done(Buffer* buf) {
    // find slot with state 'BUSY_WRITING' and set state to FULL
    int i;
    for (i=0; i<BUF_SIZE; i++) {
        if (buf->data[i] == BUSY_WRITING) {
            buf->data[i] = FULL;
            return;
        }
    }
    // error
}

