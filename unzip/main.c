#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <assert.h>

#include "miniz/miniz_tinfl.h"


static u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
}

typedef struct {
    uint8_t magic[2];
    uint8_t method;
    uint8_t flags;
    uint32_t time;
    uint8_t xflags;
    uint8_t os;
} __attribute__((packed)) gzip_header_t;

#define GZIP_MAGIC0             0x1f
#define GZIP_MAGIC1             0x8b

#define GZIP_ASCII              (1<<0)
#define GZIP_HEADER_CRC         (1<<1)
#define GZIP_EXTRA_FIELD        (1<<2)
#define GZIP_ORIG_NAME          (1<<3)
#define GZIP_COMMENT            (1<<4)
#define GZIP_ENCRYPTED          (1<<5)

#define GZIP_CRC_SIZE 8
#define GZIP_METHOD_DEFLATE     8

#define GZIP_MAX_HEADER_SIZE    512

static int is_compressed(const void* data, unsigned size, unsigned* isize)
{
    const gzip_header_t* hdr = (const gzip_header_t*)data;
    uint32_t uncompressed_size = *(uint32_t*)(data + size-4);

    if (hdr->magic[0] != GZIP_MAGIC0 || hdr->magic[1] != GZIP_MAGIC1) return 0;
    if (hdr->method != GZIP_METHOD_DEFLATE) return 0;
    if (hdr->flags & GZIP_ENCRYPTED) return 0;
    if (uncompressed_size == 0) return 0;
    *isize = uncompressed_size;

    return 1;
}

static int skip_variable_field(const void* data, unsigned* offset) {
    const uint8_t* tmp = data;
    unsigned off = *offset;
    do {
        if (off >= GZIP_MAX_HEADER_SIZE) return 0;
    } while (tmp[off++]);
    *offset = off;
    return 1;
}


static int my_callback(const void *pBuf, int len, void *pUser) {
    unsigned* total_size = (unsigned*)pUser;
    *total_size += len;
    return 1;
}

static void write_to_file(const void* data, unsigned size, const char* filename)
{
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        fprintf(stderr, "error creating %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    ssize_t written = write(fd, data, size);
    if (written != size) {
        fprintf(stderr, "error writing %s\n", strerror(errno));
    }
    close (fd);
    printf("written %s\n", filename);
}

static void decompress(const void* data, unsigned size, const char* outfile)
{
    uint32_t uncompressed_size = 0;
    if (!is_compressed(data, size, &uncompressed_size)) {
        printf("not compressed\n");
        return;
    }

    printf("Size %u -> %u (%u%%)\n\n", size, uncompressed_size, (size * 100)/uncompressed_size);

    const gzip_header_t* hdr = (const gzip_header_t*)data;
    unsigned payload_start = sizeof(gzip_header_t);
    if (hdr->flags & GZIP_EXTRA_FIELD) {
        uint16_t extra_len = *(uint16_t*)(data + payload_start);
        payload_start += (2 + extra_len);
    }
    if (hdr->flags & GZIP_ORIG_NAME) {
        if (!skip_variable_field(data, &payload_start)) return;
    }
    if (hdr->flags & GZIP_COMMENT) {
        if (!skip_variable_field(data, &payload_start)) return;
    }
    if (hdr->flags & GZIP_HEADER_CRC) payload_start += 2;

    unsigned payload_size = size - payload_start - GZIP_CRC_SIZE;

    size_t outsize = uncompressed_size;
    void* dest = malloc(outsize);
    int flags = 0;

    uint64_t t1 = getCurrentTime();
    outsize = tinfl_decompress_mem_to_mem(dest, outsize, data + payload_start, payload_size, flags);
    if (outsize == TINFL_DECOMPRESS_MEM_TO_MEM_FAILED) {
        printf("decompress failed\n");
        return;
    }
    uint64_t t2 = getCurrentTime();
    printf("decompressing mem_to_mem took %lu usec\n", t2 - t1);
    printf("uncompressed_size %lu\n", outsize);

    if (outfile) write_to_file(dest, outsize, outfile);

    printf("\n");
    uint64_t t3 = getCurrentTime();
    size_t psize = payload_size;
    uncompressed_size = 0;
    int ok = tinfl_decompress_mem_to_callback(data + payload_start, &psize, my_callback, &uncompressed_size, 0);
    uint64_t t4 = getCurrentTime();
    if (!ok) {
        printf("decompressing failed\n");
        return;
    }
    printf("decompressing mem_to_callback took %lu usec\n", t4 - t3);
    printf("uncompressed_size %u\n", uncompressed_size);
    free(dest);
}

int main(int argc, const char *argv[])
{
    if (argc < 2 || argc > 3) {
        printf("Usage %s [file] <outfile>\n", argv[0]);
        return -1;
    }
    const char* filename = argv[1];
    const char* outfile = NULL;
    if (argc == 3) outfile = argv[2];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("open"); return 0; }

    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        close(fd);
        return 0;
    }
    unsigned size = statbuf.st_size;

    void* map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); return 0; }
    close(fd);

    decompress(map, size, outfile);

    munmap(map, size);
    return 0;
}

