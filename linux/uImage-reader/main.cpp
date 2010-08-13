#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <byteswap.h>

#include <zlib.h>

#define uint32_t u_int32_t
#define uint8_t u_int8_t

#include "image.h"

#if BYTE_ORDER == LITTLE_ENDIAN
#define cpu_to_le16(val) (val)
#define cpu_to_le32(val) (val)
#define cpu_to_le64(val) (val)
#define cpu_to_be16(val) bswap_16(val)
#define cpu_to_be32(val) bswap_32(val)
#define cpu_to_be64(val) bswap_64(val)
#define le16_to_cpu(val) (val)
#define le32_to_cpu(val) (val)
#define le64_to_cpu(val) (val)
#define be16_to_cpu(val) bswap_16(val)
#define be32_to_cpu(val) bswap_32(val)
#define be64_to_cpu(val) bswap_64(val)
#elif BYTE_ORDER == BIG_ENDIAN
#define cpu_to_le16(val) bswap_16(val)
#define cpu_to_le32(val) bswap_32(val)
#define cpu_to_le64(val) bswap_64(val)
#define cpu_to_be16(val) (val)
#define cpu_to_be32(val) (val)
#define cpu_to_be64(val) (val)
#define le16_to_cpu(val) bswap_16(val)
#define le32_to_cpu(val) bswap_32(val)
#define le64_to_cpu(val) bswap_64(val)
#define be16_to_cpu(val) (val)
#define be32_to_cpu(val) (val)
#define be64_to_cpu(val) (val)
#else
#error unknwon BYTE_ORDER
#endif


int checkUImage(const void* image, unsigned int len) {
    struct image_header header;
    if (len < sizeof(header)) {
        printf("Invalid magic, not an uImage\n");
        return -1;
    }
    memcpy(&header, image, sizeof(header));

    printf("header magic: 0x%08X\n", be32_to_cpu(header.ih_magic));
    printf("header header CRC: 0x%08X\n", be32_to_cpu(header.ih_hcrc));
    printf("header time: %u\n", be32_to_cpu(header.ih_time));
    printf("header data size: %u\n", be32_to_cpu(header.ih_size));
    printf("header data load address: 0x%08X\n", be32_to_cpu(header.ih_load));
    printf("header entry point: 0x%08X\n", be32_to_cpu(header.ih_ep));
    printf("header data CRC: 0x%08X\n", be32_to_cpu(header.ih_dcrc));
    printf("header OS: %d\n", header.ih_os);
    printf("header arch: %d\n", header.ih_arch);
    printf("header type: %d\n", header.ih_type);
    printf("header comp: %d\n", header.ih_comp);
    printf("header name: %s\n", header.ih_name);

    if (be32_to_cpu(header.ih_magic) != IH_MAGIC) {
        printf("Invalid magic, not an uImage\n");
        return -1;
    }

    unsigned int hcrc = be32_to_cpu(header.ih_hcrc);
    header.ih_hcrc = 0;
    unsigned int crc = crc32(0, (const Bytef*)&header, sizeof(header));
    if (crc != hcrc) {
        printf("Header checksum of the uImage does not match\n");
        return -1;
    }

    if (header.ih_type != IH_TYPE_KERNEL) {
        printf("uImage type %d unsupported\n", header.ih_type);
        return -1;
    }

    if (header.ih_os != IH_OS_LINUX) {
        printf("uImage os %d unsupported\n", header.ih_os);
        return -1;
    }

    if (header.ih_arch != IH_ARCH_ARM) {
        printf("uImage arch %d unsupported\n", header.ih_arch);
        return -1;
    }

    if (be32_to_cpu(header.ih_size) != len - sizeof(header)) {
        printf("uImage header claims that image has %d bytes\n", be32_to_cpu(header.ih_size));
        printf("we read only %u bytes.\n", len - sizeof(header));
        return -1;
    }

    crc = crc32(0, (const Bytef*)image + sizeof(header), len - sizeof(header));
    if (crc != be32_to_cpu(header.ih_dcrc)) {
        printf("The data CRC does not match. Computed: %08x expected %08x\n", crc,
                be32_to_cpu(header.ih_dcrc));
        return -1;
    }

    return 0;
}


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [file]\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) { perror("open"); return -1; }

    struct stat statbuf;
    int err = stat(argv[1], &statbuf);
    if (err) {
        perror("stat");
        return -1;
    }
    printf("file size = %d\n", (int)statbuf.st_size);
    void* region = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (region == MAP_FAILED) { perror("mmap"); return -1; }
    close(fd);

    checkUImage(region, statbuf.st_size);

    munmap(region, statbuf.st_size);
    return 0;
}
