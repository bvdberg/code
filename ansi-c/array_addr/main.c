#include <stdio.h>

unsigned char buffer[100][100];
unsigned char buffer2[123];

int main(int argc, const char *argv[])
{
    printf("buffer = %p\n", buffer);
    printf("&buffer = %p\n", &buffer);
    printf("buffer[0] = %p\n", buffer[0]);
    printf("&buffer[0] = %p\n", &buffer[0]);
    printf("&buffer[0][0] = %p\n", &buffer[0][0]);

    printf("buffer2 = %p\n", buffer2);
    printf("&buffer2 = %p\n", &buffer2);

    return 0;
}

