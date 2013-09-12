#include <stdio.h>

static unsigned bitdiffs(unsigned char c1, unsigned char c2) {
    unsigned char v = c1 ^ c2;
    unsigned c = 0;
    // count bits in v
    while (v) {
      v &= v - 1; // clear the least significant bit set
      c++;
    }
    printf("%02x %02x -> %d\n", c1, c2, c);
    return c;
}

int main(int argc, const char *argv[])
{
    unsigned char c1 = 0x21;
    unsigned char c2 = 0x34;

    bitdiffs(c1, c2);
    return 0;
}

