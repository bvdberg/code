#ifndef CIRCBUF_H
#define CIRCBUF_H

/*
    Design:
    - if take == put, buffer is empty
    - if put == take-1, buffer is full
    -> so at most size-1 elements can be stored
*/

#define BUF_SIZE 20

typedef struct {
    unsigned int takeIndex;
    unsigned int putIndex;
    unsigned int size;
    unsigned char data[BUF_SIZE];
} Buffer;


unsigned int buffer_data(Buffer* buf);

unsigned int buffer_free(Buffer* buf);

void buffer_add(Buffer* buf, int amount);

void buffer_remove(Buffer* buf, int amount);

void buffer_print(Buffer* buf);

#endif

