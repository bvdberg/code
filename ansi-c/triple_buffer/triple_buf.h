#ifndef CIRCBUF_H
#define CIRCBUF_H

/*
    Design:
    * consumer can take some time to consume the buffer (rendering image)
    * the producer needs to keep producing (eg camera filling buffer)
    * single producer and single consumer
    * especially handy if producer/consumer run at different or nearly same speeds

    -> void read_start();
    -> int number = read_done();
    -> void write_start();
    -> write_done(int number);
*/

#define BUF_SIZE 3

enum State {
    EMPTY = 0,
    FULL,
    BUSY_READING,
    BUSY_WRITING,
};

typedef struct {
    enum State data[BUF_SIZE];
} Buffer;

void buffer_init(Buffer* buf);

void buffer_print(Buffer* buf);

// returns -1 if no buffer, slot nr otherwise
int read_start(Buffer* buf);

void read_done(Buffer* buf);

// returns -1 if no space, slot nr otherwise
int write_start(Buffer* buf);

void write_done(Buffer* buf);

#endif

