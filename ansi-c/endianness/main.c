#include <stdio.h>

typedef struct {
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
} Struct;


int main(int argc, const char *argv[])
{
    int num = 0x11223344;
    char* c = (char*)&num;
    printf("i=%d (%p)\n", num, &num);
    int i;
    for (i=0; i<4; i++) {
        printf("  [%d] 0x%02x\n", i, *(c+i));
    }
    Struct *s = (Struct*)&num;
    printf("s=%p\n", s);
    printf("a=0x%02x  (%p)\n", s->a, &s->a);
    printf("b=0x%02x  (%p)\n", s->b, &s->b);
    printf("c=0x%02x  (%p)\n", s->c, &s->c);
    printf("d=0x%02x  (%p)\n", s->d, &s->d);

    printf("\nmachine is ");
    if (s->a == 0x44) printf("little-endian\n");
    else if (s->a == 0x11) printf("big-endian\n");
    else printf("unknown endianness\n");

    return 0;
}

