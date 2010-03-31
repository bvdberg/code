/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 *
 * This handles basic git sha1 object files - packing, unpacking,
 * creation etc.
 */

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

#include "sha1.h"

#ifndef O_NOATIME
#if defined(__linux__) && (defined(__i386__) || defined(__PPC__))
#define O_NOATIME 01000000
#else
#define O_NOATIME 0
#endif
#endif

static char *sha1_to_hex(const unsigned char *sha1)
{
    static int bufno;
    static char hexbuffer[4][50];
    static const char hex[] = "0123456789abcdef";
    char *buffer = hexbuffer[3 & ++bufno], *buf = buffer;
    int i;

    for (i = 0; i < 20; i++) {
        unsigned int val = *sha1++;
        *buf++ = hex[val >> 4];
        *buf++ = hex[val & 0xf];
    }
    *buf = '\0';

    return buffer;
}


static const unsigned char null_sha1[20];
static inline int is_null_sha1(const unsigned char *sha1)
{
    return !memcmp(sha1, null_sha1, 20);
}
static inline int hashcmp(const unsigned char *sha1, const unsigned char *sha2)
{
    return memcmp(sha1, sha2, 20);
}
static inline void hashcpy(unsigned char *sha_dst, const unsigned char *sha_src)
{
    memcpy(sha_dst, sha_src, 20);
}
static inline void hashclr(unsigned char *hash)
{
    memset(hash, 0, 20);
}


static void write_sha1_file_prepare(const void *buf, unsigned long len,
                                    const char *type, unsigned char *sha1,
                                    char *hdr, int *hdrlen)
{
	git_SHA_CTX c;

	/* Generate the header */
	*hdrlen = sprintf(hdr, "%s %lu", type, len)+1;

	/* Sha1.. */
	git_SHA1_Init(&c);
	git_SHA1_Update(&c, hdr, *hdrlen);
	git_SHA1_Update(&c, buf, len);
	git_SHA1_Final(sha1, &c);
}


int hash_sha1_file(const void *buf, unsigned long len, const char *type,
                   unsigned char *sha1)
{
	char hdr[32];
	int hdrlen;
	write_sha1_file_prepare(buf, len, type, sha1, hdr, &hdrlen);
	return 0;
}


static int sha1_file(const char* filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("open"); return -1; }

    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        return -1;
    }
    int size = statbuf.st_size;
    void* map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); return -1; }
    close(fd);

    unsigned char sha1[20];
    hash_sha1_file(map, size, "type", sha1);
    printf("sha1: %s\n", sha1_to_hex(sha1));

    munmap(map, size);
    return 0;
}


int main(int argc, const char *argv[])
{
    if (argc == 1) {
        printf("Usage %s [file]\n", argv[0]);
        return -1;
    }

    sha1_file(argv[1]);
    return 0;
}
 
