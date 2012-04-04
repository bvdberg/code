#include <stdio.h>

static unsigned int buffer[8]; // 256 bits

static void setbit(unsigned char i) {
    unsigned int offset = (i & 0x1F);
    unsigned int count = i >> 5;
    buffer[count] |= (1 << offset);
}

static unsigned int is_set(unsigned char i) {
    unsigned int offset = (i & 0x1F);
    unsigned int count = i >> 5;
    unsigned int set = (buffer[count] >> offset) & 1;
    printf("bit %d = %d\n", i, set);
    return set;
}

static void print_set() {
    int i;
    for (i=0; i<256; i++) {
        unsigned int offset = (i & 0x1F);
        unsigned int count = i >> 5;
        unsigned int set = (buffer[count] >> offset) & 1;
        printf("%d ", set);
        if (i%16 == 15) printf("\n");
    }
}

int main(int argc, const char *argv[])
{
    setbit(10);
    is_set(10);
    setbit(127);
    setbit(200);
    setbit(255);
    is_set(254);
    is_set(255);
    print_set();
    
    return 0;
}

