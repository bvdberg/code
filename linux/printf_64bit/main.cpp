#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>

int main(int argc, const char *argv[])
{
    u_int64_t number = 10;
    printf("number = %"PRIu64"  specifier='"PRIu64"'\n", number);

    return 0;
}

