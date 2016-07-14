#include <stdio.h>
#include <stdint.h>

#define ASSIGN_BIT(w, pos, value) (w |= (value<<pos))

#define SET_BIT(w, pos) (w |= (1<<pos))
#define CLR_BIT(w, pos) (w &= ~(1<<pos))

int main(int argc, const char *argv[])
{
    uint32_t i = 0;
    printf("i=0x%x\n", i);
    ASSIGN_BIT(i, 3, 1);
    printf("i=0x%x\n", i);
    ASSIGN_BIT(i, 4, 0);
    printf("i=0x%x\n", i);
    CLR_BIT(i, 3);
    SET_BIT(i, 2);
    SET_BIT(i, 4);
    printf("i=0x%x\n", i);
    return 0;
}

