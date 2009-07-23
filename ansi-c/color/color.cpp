#include <stdio.h>
/*
#define ANSI_BLACK "\033[22;30m"
#define ANSI_RED "\033[22;31m"
#define ANSI_GREEN "\033[22;32m"
#define ANSI_BROWN "\033[22;33m"
#define ANSI_BLUE "\033[22;34m"
#define ANSI_MAGENTA "\033[22;35m"
#define ANSI_CYAN "\033[22;36m"
#define ANSI_GREY "\033[22;37m"

#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED "\033[01;31m"
#define ANSI_BGREEN "\033[01;32m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_BBLUE "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_NORMAL "\033[22;37m"
*/

// attributes
#define RESET       0
#define BRIGHT      1
#define DIM         2
#define UNDERLINE   4
#define BLINK       5
#define REVERSE     7
#define HIDDEN      8

// color
#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

const char* attribute2str[] = { "RESET", "BRIGHT", "DIM", "UNDERLINE", "BLINK", "REVERSE", "HIDDEN" };
const char* color2str[] = {"BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGENTA", "CYAN", "WHITE"};

void textcolor(int attr, int fg, int bg)
{
    char command[13];
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
    
    printf("esc[%d;%d;%dm", attr, fg + 30, bg + 40);
}


int main() {
    for (int i=0; i<8; i++) {
        textcolor(RESET, i, BLACK);
        printf("TEXT COLOR (RESET, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(BRIGHT, i, BLACK);
        printf("TEXT COLOR (BRIGHT, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(DIM, i, BLACK);
        printf("TEXT COLOR (DIM, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(UNDERLINE, i, BLACK);
        printf("TEXT COLOR (UNDERLINE, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(BLINK, i, BLACK);
        printf("TEXT COLOR (BLINK, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(REVERSE, i, BLACK);
        printf("TEXT COLOR (REVERSE, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    for (int i=0; i<8; i++) {
        textcolor(HIDDEN, i, BLACK);
        printf("TEXT COLOR (HIDDEN, %s, %s)\n", color2str[i], color2str[0]);
        textcolor(RESET, WHITE, BLACK);
    }
    printf("\n");
    return 0;
}

