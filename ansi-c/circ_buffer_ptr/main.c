#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_BLACK "\033[22;30m"
#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_BROWN "\033[22;33m"
#define ANSI_BLUE "\033[22;34m"
#define ANSI_MAGENTA "\033[22;35m"
#define ANSI_CYAN "\033[22;36m"
#define ANSI_GREY "\033[22;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED "\033[01;31m"
#define ANSI_BGREEN "\033[01;32m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_BBLUE "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_NORMAL "\033[22;0m"

#define NUM_BUFFERS 4
typedef struct {
    unsigned int head;
    unsigned int count;
    unsigned char* data[NUM_BUFFERS];
} Buffer;

Buffer buf;


void buffer_init() {
    buf.head = 0;
    buf.count = 0;
    int i;
    for (i=0; i<NUM_BUFFERS; i++) {
        buf.data[i] = (unsigned char*)malloc(256);
    }
}

static void buffer_print() {
    printf("  -> count=%d  head=%d\n", buf.count, buf.head);
}

static void buf_consume() {
    printf("%s() count=%d\n", __func__, buf.count);
    if (buf.count > 0) {
        printf(ANSI_GREEN"consuming %d"ANSI_NORMAL"\n", buf.head);
        buf.head = ((buf.head + 1) % NUM_BUFFERS);
        buf.count--;
    }
    buffer_print();
}

static void buf_produce() {
    printf("%s() count=%d\n", __func__, buf.count);
    if (buf.count != NUM_BUFFERS) {
        int index = (buf.head + buf.count) % NUM_BUFFERS;
        buf.count++;
        printf(ANSI_RED"producing %d"ANSI_NORMAL"\n", index);
    }
    buffer_print();
}

int main() {

    buffer_init();
    buf_consume();
    buf_produce();
    buf_produce();
    buf_produce();
    buf_produce();
    buf_consume();
    buf_produce();
    buf_consume();
    buf_consume();
    buf_consume();
    buf_consume();


    return 0;
}

