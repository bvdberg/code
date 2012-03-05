#include <stdio.h>

// from: http://www.cse.yorku.ca/~oz/hash.html

static unsigned long hash_djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        //hash(i) = hash(i - 1) * 33 ^ str[i]; // NEW
    }

    return hash;
}

static unsigned long hash_sdbm(unsigned char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}


int main(int argc, const char *argv[])
{
    if (argc != 2) return -1;
    const char* input = argv[1];
    printf("hashing %s\n", input);
    printf("djb2: %lu\n", hash_djb2((unsigned char*)input));
    printf("sdbm: %lu\n", hash_sdbm((unsigned char*)input));
    return 0;
}

