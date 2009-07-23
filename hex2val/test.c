#include <stdio.h>

static char* mac2str(const unsigned char* mac) {
    static int bufno;
    static char hexbuffer[4][50];
    static const char hex[] = "0123456789ABCDEF";
    char *buffer = hexbuffer[3 & ++bufno], *buf = buffer;
    int i;

    for (i = 0; i < 6; i++) {
        unsigned int val = *mac++;
        if (i) *buf++ = ':';
        *buf++ = hex[val >> 4];
        *buf++ = hex[val & 0xf];
    }
    *buf = '\0';
    return buffer;
}

int main() {
    unsigned char mac1[] = { 0xAB, 0x50, 0x00, 0xFF, 0x11, 0xA5 };
    unsigned char mac2[] = { 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0 };
    printf("mac1=%s\n", mac2str(mac1));
    printf("mac2=%s\n", mac2str(mac2));
    printf("mac1=%s mac2=%s\n", mac2str(mac1), mac2str(mac2));

    return 0;
}

