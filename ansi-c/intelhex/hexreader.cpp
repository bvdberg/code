#include <stdio.h>
#include <string.h>
#include <assert.h>

enum RecordType {
    TYPE_DATA=0,
    TYPE_EOF=1,
};

int calcChecksum(char* input, int len) {
    int sum = 0;
    char* ptr = input;
    while(ptr != (input + len)) {
        unsigned int val = 0;
        int result = sscanf(ptr, "%02x", &val);
        assert(result == 1);
        ptr += 2;
        sum += val;
    }
    sum ^= 0xff;    // take 2s complement
    sum += 1;
    sum &= 0xff;    // take LSB
    return sum;
}

void parseLine(char* line) {
    char* ptr = line;
    ptr++;  // skip :
   
    int bytecount = 0;
    int result = sscanf(ptr, "%02x", &bytecount);
    assert(result == 1);
    ptr+=2;  // skip bytecount

    int address = 0;
    result = sscanf(ptr, "%04x", &address);
    assert(result == 1);
    ptr+=4; // skip address

    int recordtype = 0;
    result = sscanf(ptr, "%02x", &recordtype);
    assert(result == 1);
    ptr+=2;  // skip recordtype

    ptr+= (bytecount * 2);    // skip data

    int checksum = 0;
    result = sscanf(ptr, "%02x", &checksum);
    assert(result == 1);

    int my_checksum = calcChecksum(line+1, 2 + 4 + 2 + bytecount*2);
    printf("  datalen=%02d  address=0x%04X  type=%02d  checksum=0x%02X (%s)\n", bytecount, address, recordtype, checksum,
        (checksum == my_checksum) ? "OK" : "ERROR");
}

int main() {
    char* lines[] = {
         ":100000000200ADFFFFFFFFFFFFFFFFFFFFFFFFFF4E"
        ,":1028800000780912080A740C1205F81228BE120505"
        ,":10A5A000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBB"
        ,":10A5B000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAB"
        ,":020000040001F9"
    };
    for (int i=0; i<5; i++) {
        char* input = lines[i];
        printf("%s        len=%d", input, strlen(input));

        parseLine(input);
    }
    return 0;
}

