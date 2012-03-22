#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char* menu  = 
"Thales HPP_Audio,                                                                     \n"
"----------------------------|----------------------------|----------------------------\n"
" Audio Input Reg [i]  :     | Codec Registers            | Audio Output Reg[o]  :     \n"
"----------------------------|----------------------------|----------------------------\n"
" loopback 1+2    [l]  :     |                            | enable output1  [1]  :     \n"
" input gain (dB) [g]  :     |                            | enable output2  [2]  :     \n"
" bias current    [b]  :     |                            | gain1 (dB)      [3]  :     \n"
"                            |                            | gain2 (dB)      [4]  :     \n"
"                            |                            | coupling1       [5]  :     \n"
"                            |                            | coupling2       [6]  :     \n"
"----------------------------|----------------------------|----------------------------\n";

#define LINE 87
#define COL1 24
#define COL3 82
#define OFFSET(l,c) (LINE*l + c)

static char menu_copy[1024];

static void menu_setvalue(const char* input, int pos, int len) {
    const char* ip = input;
    char* op = menu_copy + pos;
    while (len) {
        if (*ip == 0) break;
        *op++ = *ip++;
        len--;
    }
    while (len) {
        *op++ = ' ';
        len--;
    }
}


static void menu_print(int pos, int len, const char *fmt, ...)
{
    va_list args;
    char printbuffer[40];
    memset(printbuffer, 0, sizeof(printbuffer));
    va_start(args, fmt);
    vsprintf(printbuffer, fmt, args);
    va_end(args);
    
    menu_setvalue(printbuffer, pos, len);
} 


static void menu_init() {
    strcpy(menu_copy, menu);
#ifdef DEBUG
    menu_setvalue("DEBUG inits :", OFFSET(0, 65), 20); // debug
#endif
}

int main(int argc, const char *argv[])
{
    menu_init();
    // top bar
    menu_setvalue("NO McASP", OFFSET(0, 18), 8);    // McASP
#ifdef DEBUG
        menu_setvalue("1025", OFFSET(0, 79), 6); // inits
#endif
    // audio input
    menu_setvalue("E1",  OFFSET(2, COL1), 2);       // input reg
    menu_setvalue("off", OFFSET(4, COL1), 3);       // loopback
    menu_setvalue("22",  OFFSET(5, COL1), 3);       // gain
    menu_setvalue("med",    OFFSET(6, COL1), 3);    // bias
    // audio output
    menu_setvalue("G3",  OFFSET(2, COL3), 2);   // output reg
    menu_setvalue("0",  OFFSET(4, COL3), 1);    // enable1
    menu_setvalue("1",  OFFSET(5, COL3), 1);    // enable2
    menu_setvalue("36",  OFFSET(6, COL3), 2);   // gain1
    menu_setvalue("0",  OFFSET(7, COL3), 2);   // gain2
    menu_setvalue("DC",  OFFSET(8, COL3), 2);   // coupling1
    menu_setvalue("AC",  OFFSET(9, COL3), 2);   // coupling2

    menu_print(OFFSET(6, COL1), 3, "%3f", 3.1415);

    printf("%s\n", menu_copy);

    return 0;
}

