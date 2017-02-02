#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_MAX 20

#define NEXT(x) ((x + 1) % QUEUE_MAX)
// head == tail means empty
// insert at head
// remove from tail

static uint32_t data[QUEUE_MAX];
static unsigned tail = 0;
static unsigned head = 0;

static bool queue_empty() { return head == tail; }

static bool queue_full() { return NEXT(head) == tail; }

static void queue_add(uint32_t item) {
    printf("add %d  tail %d  head %d\n", item, tail, head);
    unsigned next = NEXT(head);
    if (tail == next) {
    	printf(" -> overflow\n");
        return;
    }
    data[head] = item;
    head = next;
}

static uint32_t queue_remove() {
    //printf("remove   tail %d  head %d\n", tail, head);
    if (head == tail) {
        printf(" -> empty\n");
        return 0;
    }
    uint32_t item = data[tail];
    tail = NEXT(tail);
    return item;
}

static void queue_show() {
    if (queue_empty()) {
        printf("empty\n");
    } else {
        printf("queue:  head %u  tail %u\n", head, tail);
        unsigned pos = tail;
        while (pos != head ) {
            printf("   %u\n", data[pos]);
            pos = NEXT(pos);
        }
    }
}

int main(int argc, const char *argv[])
{
	queue_show();
    for (unsigned i=0; i<QUEUE_MAX+1; i++) {
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

