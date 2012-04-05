#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


#define NR_RTABLE_IN 100

typedef struct {
    unsigned int port;
    unsigned int chid;
} rtable_in;

static rtable_in rt_in[NR_RTABLE_IN];
static unsigned int num_rt_in;

// linear: total = 36190   avg = 361
// binary: total = 19162   avg = 191
static int rtable_lookup(unsigned int port) {
#if 0
    // linear search
    unsigned int i;
    for (i=0; i<num_rt_in; i++) {
        if (rt_in[i].port == port) return i;
    }
#else
    // binary search
    unsigned int i = num_rt_in / 2;
    unsigned int lower = 0;
    unsigned int upper = num_rt_in;
    while (upper != lower) {
        unsigned int p = rt_in[i].port;
        //printf("  lower=%d  upper=%d  i=%d  port=%d  p[i]=%d\n", lower, upper, i, port, p);
        if (p == port) return i;
        if (p < port) { // search right part
            lower = i;
        } else {    // search left part
            upper = i;
        }
        i = (upper + lower)/2;
    }
#endif
    return -1;
}


static void rtable_add(unsigned int port, unsigned int chid) {
    if (num_rt_in == NR_RTABLE_IN) {
        printf("Cannot add %d, table full\n", port);
        return; // -ENOSPC
    }

    // add sorted (start from end)
    int i = num_rt_in - 1;
    int j;
    while (i>=0) {
        if (rt_in[i].port < port) break;
        i--;
    }
    i++;    // add after found position
    if (rt_in[i].port == port) {
        printf("error, already have port %d\n", port);
        return; // TODO ERRDUP?
    }
    // move pos+1 - end
    for (j=num_rt_in; j>i; j--) {
        rtable_in* src = &rt_in[j-1];
        rtable_in* dst = &rt_in[j];
        dst->port = src->port;
        dst->chid = src->chid;
    }
    
    // insert after pos i
    rtable_in* entry = &rt_in[i];
    entry->port = port;
    entry->chid = chid;
    num_rt_in++;
    // return 0;
}


static void rtable_del(unsigned int port, unsigned int chid) {
    printf("del: port %d\n", port);
    int index = rtable_lookup(port);
    if (index == -1) {
        printf("  del: no such port %d\n", port);
        return;
    }
    if (rt_in[index].chid != chid) {
        printf("  del: port %d no matching chid\n", port);
        return;
    }
    while ((unsigned int)index < (num_rt_in-1)) {
        printf("  moving %d -> %d\n", index+1, index);
        rtable_in* src = &rt_in[index+1];
        rtable_in* dst = &rt_in[index];
        dst->port = src->port;
        dst->chid = src->chid;

        index++;
    }
    num_rt_in--;
    rt_in[num_rt_in].port = 0;
    rt_in[num_rt_in].chid = 0;
}


// ---------------------------------------------------------------------

static void rtable_print(void) {
    unsigned int i;
    for (i=0; i<num_rt_in; i++) {
        rtable_in* t = &rt_in[i];
        printf("[%3d] port=%3d  chid=%d\n", i, t->port, t->chid);
    }
}


int debug_find(int port) {
    printf("looking for %d\n", port);
    int i = rtable_lookup(port);
    printf("find %d -> index %d\n", port, i);
    return i;
}


int main(int argc, const char *argv[])
{
#if 1
    u_int64_t total = 0;
    int i;
    for (i=1; i<=NR_RTABLE_IN; i++) {
        rtable_add(i, i+1000);
    }
    //rtable_print();
    u_int64_t t1 = rdtsc();
    for (i=1; i<=NR_RTABLE_IN; i++) {
        int index = rtable_lookup(i);
        if (index != i-1) {
            printf("wrong index\n");
            break;
        }
    }
    u_int64_t t2 = rdtsc();
    total += (t2 - t1);
    printf("total = %llu   avg = %llu\n", total, total / NR_RTABLE_IN);
#else
    debug_find(10);
    rtable_add(10, 1230);
    rtable_add(20, 1230);
    rtable_add(30, 1230);
    debug_find(30);
    rtable_add(10, 1230);
    rtable_add(20, 1230);
    rtable_add(15, 1230);
    rtable_add(15, 1230);
    rtable_add(5, 1230);
    rtable_add(35, 1230);
    rtable_add(40, 1230);
    rtable_print();
    debug_find(10);
    debug_find(11);
    debug_find(40);
    debug_find(100);
    rtable_del(10, 1231);   // wrong chid
    rtable_del(5, 1230);   // ok, first
    rtable_del(40, 1230);   // ok, last
    rtable_del(20, 1230);   // ok, middle
    rtable_del(10, 1230);
    rtable_del(35, 1230);
    rtable_del(15, 1230);
    rtable_print();
#endif
    return 0;
}

