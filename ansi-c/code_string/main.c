#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
//    const unsigned char* key = "1234567890abcdef";
//    const unsigned char key[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 0 };
    unsigned char key[17];
    key[0] = '1';
    key[1] = '2';
    key[2] = '3';
    key[3] = '4';
    key[4] = '5';
    key[5] = '6';
    key[6] = '7';
    key[7] = '8';
    key[8] = '9';
    key[9] = '0';
    key[10] = 'a';
    key[11] = 'b';
    key[12] = 'c';
    key[13] = 'd';
    key[14] = 'e';
    key[15] = 'f';
    key[16] = 0;
    unsigned char buffer[256];
    strcpy(buffer, key);

    int len = strlen(key);
    int i;
    unsigned char mod = 0x5A;
    for (i=0; i<len; i++) {
        buffer[i] ^= mod;
        mod++;
    }
    printf("key = [");
    for (i=0; i<len; i++) printf("%02x ", buffer[i]);
    printf("]\n");
    printf("secret key='%s'  buffer=[%s]\n", key, buffer);
    return 0;
}

