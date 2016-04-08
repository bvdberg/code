#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

/*
    - only search sub-results for next char
        -> remember match-len, just do  match[matchlen] == newchar
*/

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
#define ANSI_NORMAL "\033[0m"

static const char* words[] = {
    "apple",
    "ananas",
    "banana",
    "cherry",
    "coconut",
    "cucumber",
    "lemon",
    "mango",
    "melon",
    "orange",
    "peach",
    "pear",
    "pineapple",
    "prune",
    "raspberry",
    "strawberry",
};
static unsigned NUM_WORDS = sizeof(words) / sizeof(words[0]);

static struct termios oldT, newT;

void initTerm() {
#ifdef __APPLE__
    tcgetattr(0, &oldT);
#else
    ioctl(0, TCGETS, &oldT);
#endif
    newT=oldT;
    newT.c_lflag &= ~ECHO;
    newT.c_lflag &= ~ICANON;
    newT.c_cc[VMIN] = 1;
    newT.c_cc[VTIME] = 0;

#ifdef __APPLE__
    tcsetattr(0, 0, &newT);
#else
    ioctl(0, TCSETS, &newT);
#endif
}

void resetTerm() {
#ifdef __APPLE__
    tcsetattr(0, 0, &oldT);
#else
    ioctl(0,TCSETS,&oldT);
#endif
}

unsigned char readChar() {
    unsigned char c;
    int res = read(0, &c, 1);
    if (res == 1 ) {
        return c;
    }
    return 0;
}

#define MAX_LEN 60
static char buffer[MAX_LEN];
static unsigned buflen;
#define MAX_MATCHES 20
static const char* matches[MAX_MATCHES];
static unsigned currentMatch;
static unsigned num_matches;

#define BACKSPACE 0x7F
#define SPACE     0x20
#define NEWLINE   0x0a
#define TAB       0x09
#define ESCAPE    0x1b

static unsigned matchLen(const char* word1, const char* word2) {
    unsigned len = 0;
    while (1) {
        if (word1[len] == 0) break;
        if (word2[len] == 0) break;
        if (word1[len] != word2[len]) break;
        len++;
    }
    return len;
}

static void findMatches() {
    // very basic matching, nothing smart
    currentMatch = -1;
    num_matches = 0;
    memset(matches, 0, sizeof(matches));
    if (buffer[0] == 0) return;

    for (unsigned i=0; i<NUM_WORDS; i++) {
        unsigned len = matchLen(words[i], buffer);
        if (len == buflen) {
            matches[num_matches] = words[i];
            num_matches++;
            if (num_matches == 1) currentMatch = 0;
        }
    }
}

static int handle(char key) {
    switch (key) {
    case BACKSPACE:
        if (buflen == 0) return 0;
        buflen--;
        buffer[buflen] = 0;
        return 1;
    case TAB:
        // select next match
        if (num_matches > 1) {
            currentMatch++;
            if (currentMatch == num_matches) currentMatch = 0;
        }
        return 0;
    case SPACE:
        if (num_matches == 0) return 0;
        strcpy(buffer, matches[currentMatch]);
        buflen = strlen(buffer);
        return 1;
    default:
        break;
    }
    if (buflen == MAX_LEN) return 0;
    buffer[buflen] = key;
    buflen++;
    buffer[buflen] = 0;
    return 1;
}

static void print() {
    char line[80];
    memset(line, ' ', sizeof(line));
    line[79] = 0;

    // print word
    char* cp = line;
    cp += sprintf(cp, ANSI_NORMAL);
    memcpy(cp, buffer, buflen);
    cp += buflen;
    // print match
    if (num_matches > 0) {
        const char* matchStr = matches[currentMatch];
        unsigned len = strlen(matchStr);
        if (len > buflen) {
            cp += sprintf(cp, ANSI_GREEN);
            memcpy(cp, matchStr + buflen, len - buflen);
            cp += (len - buflen);
        }
    }
    cp += sprintf(cp, ANSI_NORMAL);
    *cp = ' ';

    int visible = num_matches ? strlen(matches[currentMatch]) : buflen;
    int numspaces = 20-visible;
    cp += numspaces;
    switch (num_matches) {
    case 0:
        if (buflen == 0) {
            const char* enter= "enter input  ";
            memcpy(cp, enter, strlen(enter));
            cp += strlen(enter);
        } else {
            const char* nomatch = "no match     ";
            memcpy(cp, nomatch, strlen(nomatch));
            cp += strlen(nomatch);
        }
        break;
    case 1:
        cp += sprintf(cp, "1/1 match    ");
        break;
    default:
        cp += sprintf(cp, "%d/%d matches  ", currentMatch+1, num_matches);
        break;
    }
    *cp = 0;
    printf("\r%s", line);
    fflush(stdout);
}

int main() {
    initTerm();

    memset(buffer, 0, sizeof(buffer));

    print();
    while (1) {
        unsigned char c = readChar();
        //printf("read '%c' (%x)\n", c, c);
        if (c == ESCAPE) break;
        if (c == NEWLINE) continue;
        int rematch = handle(c);
        if (rematch) findMatches();
        print();
    }

    resetTerm();
    printf("\n");
    return 0;
}

