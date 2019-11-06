#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>

#include <elf.h>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

static const char* class2str(uint8_t c) {
    switch (c) {
    case 0:     return "none";
    case 1:     return "32-bit";
    case 2:     return "64-bit";
    default:
        break;
    }
    return "?";
}

static const char* mach2str(uint16_t m) {
    switch (m) {
    case EM_386:        return "x86";
    case EM_X86_64:     return "x86_64";
    default:
        break;
    }
    return "?";
}

const uint8_t MAGIC[4] = { 0x7F, 'E', 'L', 'F' };

static int elf_parse(const void* data, unsigned size)
{
    // TODO sanity check size
    const Elf32_Ehdr* H = (const Elf32_Ehdr*)data;;
    if (memcmp(H->e_ident, MAGIC, sizeof(MAGIC)) != 0) {
        printf("no ELF magic\n");
        return 1;
    }
    printf("class: %u (%s)\n", H->e_ident[EI_CLASS], class2str(H->e_ident[EI_CLASS]));
    if (H->e_ident[EI_DATA] != ELFDATA2LSB) return 1;
    
    printf("machine %u\n", H->e_machine);
    printf("version %u\n", H->e_version);
    printf("type %u\n", H->e_type);
    printf("entry addr 0x%x\n", H->e_entry);

    printf("header offset: 0x%x (%u)\n", H->e_phoff, H->e_phoff);
    printf("  size %u  count %u\n", H->e_phentsize, H->e_phnum);

    printf("section offset: 0x%x (%u)\n", H->e_shoff, H->e_shoff);
    printf("  size %u  count %u\n", H->e_shentsize, H->e_shnum);

    // load address
    const char* phdr_base = (const char *) data + H->e_phoff;
#define phdr(i)         ((Elf32_Phdr *) (phdr_base + (i) * H->e_phentsize))

    unsigned lowest = ~0;
    unsigned highest = 0;
    for (unsigned i=0; i<H->e_phnum; i++) {
        const Elf32_Phdr* ph = phdr(i);
        printf("  %2u  %u  off 0x%09x  p_addr 0x%08x  filesz 0x%08x  memsz 0x%08x\n",
            i, ph->p_type, ph->p_offset, ph->p_paddr, ph->p_filesz, ph->p_memsz);
        if (ph->p_type == PT_LOAD) {
            lowest = MIN(lowest, ph->p_paddr);
            highest = MAX(highest, ph->p_paddr + ph->p_memsz);
        }
    }
    unsigned load_size = highest - lowest;
    printf("LOAD 0x%08x - 0x%08x (%u)\n", lowest, highest, load_size);

    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        printf("Usage %s [file]\n", argv[0]);
        return -1;
    }
    const char* filename = argv[1];

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

    printf("file-size: %u\n", size);
    elf_parse(map, size);

    munmap(map, size);
    return 0;
}

