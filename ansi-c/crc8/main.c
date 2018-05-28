#include <stdio.h>
#include <stdint.h>

// based on: http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html

static uint8_t crc8(const uint8_t* input, uint8_t len)
{
    const uint8_t generator = 0x1D;
    uint8_t crc = 0;

	for (unsigned i=0; i<len; i++)
    {
        crc ^= input[i];

        for (int i = 0; i < 8; i++) {
            if ((crc & 0x80) != 0) {
                crc = (uint8_t)((crc << 1) ^ generator);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

static void test(const uint8_t* data, uint8_t len)
{
    uint8_t crc = crc8(data, len);
    printf("CRC [%d] 0x%02X -> 0x%02X\n", len, data[0], crc);
}

int main(int argc, const char *argv[])
{
    uint8_t input1[] = { 0x14 };
    test(input1, 1);

    uint8_t input2[] = { 0x5c };
    test(input2, 1);

	printf("\n");

	for (unsigned i=0; i<256; i++) {
		uint8_t v = i;
		test(&v, 1);
	}
    return 0;
}

