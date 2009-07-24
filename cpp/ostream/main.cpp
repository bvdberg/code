#include <ostream>
#include <sstream>
#include <iostream>

using namespace std;




#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}



char* itoa(int value) {
    static char str[12];
    if (value >= 0) return itoa((unsigned int)value);

    char* number = itoa((unsigned int)(-value));
    str[0] = '-';
    strcpy(&str[1], number);
    return str;
}


char* itoa(unsigned int value) {
    static char str[11];
    str[10] = '\0';
    int  pos = 9;
    do {
        str[pos--] = (char) ((value % 10) + 0x30);
        value /= 10;
    } while (value != 0);
    return str + pos + 1;
}


string itos(unsigned int value) {
    return (string)itoa(value);
}


int main() {

    stringstream stream;
    int num = 123456;
    string numstr;
    string numstr2;

    uint64_t one = rdtsc();
    stream <<  num;
    numstr = stream.str();
    uint64_t two = rdtsc();
    uint64_t diff = two - one;

    one = rdtsc();
    sleep(1);
    two = rdtsc();
    uint64_t cycles = two - one;

    double duration = (1000.0 * diff ) / cycles;
    printf("stringstream duration = %lf ms\n", duration);

    one = rdtsc();
    numstr2 = itoa(num);
    two = rdtsc();
    diff = two - one;

    double duration2 = (1000.0 * diff ) / cycles;
    printf("itoa duration = %lf ms\n", duration2);
    return 0;
}

