#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// run with strace a.out   or   ltrace a.out

struct header
{
    size_t length;
    size_t magic;
} __packed;

#define MALLOC_MAGIC 0xfeedbeaf

int main() {
    int i;
    struct header* hdr = (struct header*)malloc(100);
    struct header* hdr2 = hdr -1;
    printf("res=%p  length=%d\n", hdr, hdr2->length);
    void* mem2 = malloc(134000);
    void* mem3 = malloc(134000);

//    sleep(1);
    return 0;
}
