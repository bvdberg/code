#include <stdio.h>
#include <string.h>

typedef struct {
    int a;
    int b;
    int c;
} Buffer;

static void func(const Buffer* b, void* buf) {
    Buffer* bb = buf;
    *bb = *b;   // copy Buffer b into buf
}

int main(int argc, const char *argv[])
{
    Buffer b = { 10, 20, 30 };
    unsigned char buffer[12];
    memset(buffer, 0, sizeof(buffer));
    func(&b, (void*)buffer);

    unsigned int i;
    for (i=0; i<sizeof(buffer); i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    return 0;
}

