#include <stdio.h>
#include <stdlib.h>

#include "FileMap.h"

/*
    - store as array of unsigned ints (3 bytes used)
    - only store even addresses (uneven are MSBs of word)



*/

#define NUM_BLOCKS 344
#define NUM_WORDS 64

static unsigned int* memory;

// store as even value
static void store(int addr, unsigned int data) {
    data |= 0xFF000000; // set top byte to FF to indicate valid value
    if (addr % 2 == 1) {
        fprintf(stderr, "error\n");
        exit(-1);
    }
    addr >>= 1;
    memory[addr] = data;
}

static unsigned int get(int addr) {
    addr >>= 1;
    return memory[addr];
}

static unsigned char hex2val(unsigned char cp) {
    switch (cp) {
    case '0': return 0x0;
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0x4;
    case '5': return 0x5;
    case '6': return 0x6;
    case '7': return 0x7;
    case '8': return 0x8;
    case '9': return 0x9;
    case 'a': case 'A': return 0xa;
    case 'b': case 'B': return 0xb;
    case 'c': case 'C': return 0xc;
    case 'd': case 'D': return 0xd;
    case 'e': case 'E': return 0xe;
    case 'f': case 'F': return 0xf;
    default:
        fprintf(stderr, "invalid char %c\n", cp);
        exit(-1);
    }
}

static unsigned char byte2val(unsigned char* cp) {
    unsigned int result = hex2val(cp[0]);
    result <<= 4;
    result &= 0xF0;
    result |= hex2val(cp[1]);
    return result;
}

static int readLine(unsigned char** input, unsigned char* output) {
    int size = 0;
    unsigned char* cp = *input;
    if (*cp != ':') {
        fprintf(stderr, "unexpected char\n");
        exit(-1);
    }
    cp++;
    // convert hex to binary
    while (1) {
        if (*cp == '\n' || *cp == '\r') {
            while (*cp == '\n' || *cp == '\r') cp++;
            *output = 0;
            *input = cp;
            break;
        } else {
            *output = byte2val(cp);
            output++;
            size++;
            cp += 2;
        }
    }
    return size;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return -1;
    }

    FileMap f(argv[1]);
    f.open();

    memory = (unsigned int*)calloc(NUM_BLOCKS * NUM_WORDS, 4);

    unsigned char* cp = (unsigned char*) f.region;
    unsigned char* end = cp + f.size;
    int line_nr = 0;
    unsigned int addr_high = 0;
    while (cp < end) {
        // read line
        line_nr++;
        unsigned char line[64];
        int len = readLine(&cp, line);
        // handle line
        unsigned int addr_low = (((int)line[1]) << 8) + line[2];
        unsigned char record = line[3];
        switch (record) {
        case 0: // data record
            {
                unsigned char byte_count = line[0];
                unsigned int addr = (addr_high << 16) | addr_low;
                addr >>= 1;  // divide by 2, because PIC has word access
                // each data line contains X 32 bits instructions
                int offset = 4;
                for (int i=0; i<byte_count/4; i++) {
                    // MSB
                    unsigned int word = 0;
                    word |= (((int)line[offset++]) <<  0);
                    word |= (((int)line[offset++]) <<  8);
                    word |= (((int)line[offset++]) << 16);
                    word |= (((int)line[offset++]) << 24);
                    word &= 0x00FFFFFF;
                    store(addr, word);
                    addr += 2;
                }
            }
            break;
        case 1: // EOF
            //printf("EOF\n");
            break;
        case 4: // extended linear address record
            // read addr_high in 2 bytes
            addr_high = line[4] * 256 + line[5];
            //printf("Extended 0x%04x\n", addr_high);
            break;
        }
    }
    for (int i=0; i<NUM_BLOCKS; i++) {
        bool needed = false;
        int base_addr = i*NUM_WORDS*2;
        for (int j=0; j<NUM_WORDS; j++) {
            int addr = base_addr + j*2;
            //printf("addr=0x%x\n", addr);
            if (get(addr) & 0xFF000000) {
                needed = true;
                break;
            }
        }
        if (needed) {
            printf("program 64 words @ %06x\n", base_addr);
            printf("   ");
            for (int j=0; j<NUM_WORDS; j++) {
                int addr = base_addr + j*2;
                //printf("addr=0x%x\n", addr);
                if (get(addr) & 0xFF000000) printf("1");
                else printf("0");
            }
            printf("\n");
        }
    }

    free(memory);

    return 0;
}

