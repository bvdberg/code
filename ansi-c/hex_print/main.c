#include <stdio.h>

static void print_line(unsigned char* buffer,  int offset, int num) {
    static char hexval[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned char line[80];
    memset(line, ' ', sizeof(line));
    line[79] = 0;
    int i;
    unsigned char* hex_cp = &line[0];
    unsigned char* char_cp = &line[16 * 3 + 4];
    for (i=0; i< num; i++) {
        unsigned char input = buffer[offset + i];
        *hex_cp++ = hexval[input >> 4];
        *hex_cp++ = hexval[input & 0x0F];
        hex_cp++;
        *char_cp++ = (isprint(input) ? input : '.');
    }
    printf("%s\n", line);
}

static void print_buffer(unsigned char* buffer, int buffer_len) {
    int offset = 0;
    while (offset < buffer_len) {
        int remaining = buffer_len - offset;
        print_line(buffer, offset, (remaining >= 16) ? 16 : remaining);
        offset += 16;
    }
}

static void print_memory(const char* name, unsigned char* start, int size) {
    printf("%9s: ", name);
    print_buffer(start, size);
}


int main(int argc, const char *argv[])
{
 
 return 0;
}
 
