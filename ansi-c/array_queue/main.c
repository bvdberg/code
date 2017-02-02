#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE 20

#define NEXT(x) ((x + 1) % QUEUE_SIZE)

static uint32_t data[QUEUE_SIZE];
static unsigned head = 0;
static unsigned size = 0;

static bool queue_empty() { return size == 0; }

static bool queue_full() { return size == QUEUE_SIZE; }

static void queue_add(uint32_t item) {
    if (size == QUEUE_SIZE) {
    	printf(" -> overflow\n");
        return;
    }
    unsigned pos = (head + size) % QUEUE_SIZE;
    data[pos] = item;
    size++;
}

static uint32_t queue_remove() {
    if (size == 0) {
        printf(" -> empty\n");
        return 0;
    }
    uint32_t item = data[head];
    head = NEXT(head);
    size--;
    return item;
}

static void queue_show() {
    if (queue_empty()) {
        printf("empty\n");
    } else {
        printf("queue:  head %u  size %u\n", head, size);
        unsigned pos = head;
        for (unsigned i=0; i<size; i++) {
            printf("   %u\n", data[pos]);
            pos = NEXT(pos);
        }
    }
}

int main(int argc, const char *argv[])
{
	queue_show();
    for (unsigned i=0; i<QUEUE_SIZE+1; i++) {
        queue_add(i);
    }
    printf("full: %d\n", queue_full());
	queue_show();
    queue_remove();
    queue_remove();
    queue_remove();
	queue_show();
    queue_add(1234);
    while (!queue_empty()) {
        queue_remove();
    }
	queue_show();
    return 0;
}

