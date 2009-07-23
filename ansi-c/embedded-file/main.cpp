#include <stdio.h>

extern char _binary_data_txt_start;
extern char _binary_data_txt_end;
extern char _binary_data_txt_size;

int main() {
    char* size = &_binary_data_txt_size;
    printf("size is %d (calculated %d)\n", (int)size, &_binary_data_txt_end - &_binary_data_txt_start);

    char* p = &_binary_data_txt_start;

    while ( p != &_binary_data_txt_end ) putchar(*p++);
}

