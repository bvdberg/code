#include <stdio.h>

int main(int argc, char* argv[])
{
    for(int i=0; i<256; i++) printf("code %3d -> [%c]\n", i, (char)i);

    return 0;
}

