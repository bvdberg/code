#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX 20

static uint8_t queue[MAX];

static void dump(void) {
    char tmp[MAX*3];
    char* cp = tmp;
    for (unsigned i=0; i<MAX; i++) {
        if (queue[i] == MAX) {
            cp += sprintf(cp, "--");
            break;
        }
        cp += sprintf(cp, "%2d ", queue[i]);
    }
    printf("%s\n", tmp);
}

static void add(uint8_t value) {
    printf("--- add %u ---\n", value);
    uint8_t i = 0;
    while (queue[i] < value) i++;
    // insert + shift rest
    while (value != MAX) {
        uint8_t tmp = queue[i];
        if (tmp == value) break;
        queue[i] = value;
        value = tmp;
        i++;
    }
    dump();
}

static void del(uint8_t value) {
    printf("--- del %u ---\n", value);
    uint8_t i = 0;
    while (queue[i] != value && queue[i] != MAX) i++;
    // shift rest
    while (queue[i] != MAX) {
        queue[i] = queue[i+1];
        i++;
    }

    dump();
}

int main(int argc, const char *argv[])
{
    memset(queue, MAX, sizeof(queue));
    dump();
    add(4);
    add(6);
    add(7);
    add(5);
    add(5);
    add(2);

    del(8);
    del(2);
    del(7);
    del(5);
    return 0;
}

