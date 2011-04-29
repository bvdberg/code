// print hex value manually (for embedded targets)
#include <stdio.h>
#include <stdlib.h>

void uart_putc(char c)
{
    printf("%c", c);
}


char hexvalues[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void printHex(unsigned int value)
{
    int i;
    for (i=28; i>= 0; i-= 4) {
        unsigned char hex = (value >> i) & 0xF;
        uart_putc(hexvalues[hex]);
    }
}

int main(int argc, const char *argv[])
{
    int num = atoi(argv[1]);
    printHex(num);    
    printf("\n");
    fflush(stdout);
    printf("TEST %d -> 0x%08x\n", num, num);
    return 0;
}
