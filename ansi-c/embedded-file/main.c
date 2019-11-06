#include <stdio.h>
#include <stdint.h>

extern char _binary_data_txt_start;
extern char _binary_data_txt_end;
extern char _binary_data_txt_size;

static void dump(const char* name, void* data) {
    const uint8_t* cp = (uint8_t*)&data;
    printf("%6s:  (ptr %p)", name, data);
    for (unsigned i=0; i<8; i++) {
        printf(" %02X", cp[i]);
    }
    printf("\n");
}

int main() {
    uint32_t size = (uint32_t)(&_binary_data_txt_end - &_binary_data_txt_start);
    printf("end - start size %u (0x%x)\n", size, size);
    // NOTE: pointer is value

    // TODO cannot understand size, last x bits are correct, but how many depends on size?!
    //printf("embed size %u\n", (uint32_t)&_binary_data_txt_size);

    dump("end", &_binary_data_txt_end);
    dump("size", &_binary_data_txt_size);
    dump("start", &_binary_data_txt_start);

    const char* p = &_binary_data_txt_start;

    // DATA seems to be 0-terminated
    printf("DATA:\n%s\n", p);
}

