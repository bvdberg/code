#include <stdio.h>
#include <stdint.h>

static const uint8_t lookup[256] = {
    255,
    ['a'] = 10, ['b'] = 11, ['c'] = 12, ['d'] = 13, ['e'] = 14, ['f'] = 15,
    255,
    ['A'] = 10, ['B'] = 11, ['C'] = 12, ['D'] = 13, ['E'] = 14, ['F'] = 15,
    255,
    ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4,
    ['5'] = 5, ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9,
    255,
};

static uint64_t hex2dec(const char* hexstr) {
    uint64_t result = 0;
    const uint8_t* cp = (uint8_t*)hexstr;
    if (cp[0] == '0' && cp[1] == 'x') cp += 2;
    while (*cp != 0) {
        const uint8_t value = lookup[*cp];
        if (value == 255) {
            //printf("invalid char '%c'\n", *cp);
            return result;
        }
        result = (result << 4) + value;
        cp++;
    }
    return result;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("usage: %s <hex>\n", argv[0]);
        return -1;
    }
    const char* input = argv[1];

    // NOTE %llx on 32-bit systems, %lx on 64-bit systems
    printf("'%s' -> 0x%llx\n", input, hex2dec(input));


    return 0;
}

