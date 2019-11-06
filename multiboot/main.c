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

#include "color.h"
#include "multiboot2.h"

static const char* tag2str(uint32_t type) {
    switch (type) {
    case MULTIBOOT_TAG_TYPE_END:                return "end";
    case MULTIBOOT_TAG_TYPE_CMDLINE:            return "cmdline";
    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:   return "boot_loader_name";
    case MULTIBOOT_TAG_TYPE_MODULE:             return "module";
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:      return "basic_meminfo";
    case MULTIBOOT_TAG_TYPE_BOOTDEV:            return "bootdev";
    case MULTIBOOT_TAG_TYPE_MMAP:               return "mmap";
    case MULTIBOOT_TAG_TYPE_VBE:                return "vbe";
    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:        return "framebuffer";
    case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:       return "elf_sections";
    case MULTIBOOT_TAG_TYPE_APM:                return "apm";
    case MULTIBOOT_TAG_TYPE_EFI32:              return "efi32";
    case MULTIBOOT_TAG_TYPE_EFI64:              return "efi64";
    case MULTIBOOT_TAG_TYPE_SMBIOS:             return "smbio";
    case MULTIBOOT_TAG_TYPE_ACPI_OLD:           return "acpi_old";
    case MULTIBOOT_TAG_TYPE_ACPI_NEW:           return "acpi_new";
    case MULTIBOOT_TAG_TYPE_NETWORK:            return "network";
    case MULTIBOOT_TAG_TYPE_EFI_MMAP:           return "mmap";
    case MULTIBOOT_TAG_TYPE_EFI_BS:             return "bs";
    case MULTIBOOT_TAG_TYPE_EFI32_IH:           return "efi32_IH";
    case MULTIBOOT_TAG_TYPE_EFI64_IH:           return "efi64_IH";
    case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:     return "load_base_addr";
    }
    return "unknown";
}
static const char* headertag2str(uint16_t type) {
    switch (type) {
    case MULTIBOOT_HEADER_TAG_END:                  return "end";
    case MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST:  return "info_request";
    case MULTIBOOT_HEADER_TAG_ADDRESS:              return "addr";
    case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS:        return "entry_addr";
    case MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS:        return "console_flags";
    case MULTIBOOT_HEADER_TAG_FRAMEBUFFER:          return "framebuffer";
    case MULTIBOOT_HEADER_TAG_MODULE_ALIGN:         return "module_align";
    case MULTIBOOT_HEADER_TAG_EFI_BS:               return "efi_bs";
    case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32:  return "addr_efi32";
    case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64:  return "addr_efi64";
    case MULTIBOOT_HEADER_TAG_RELOCATABLE:          return "relocatable";
    }
    return "unknown";
}

static unsigned tag_padding(unsigned tagsize) {
    unsigned tail = tagsize % 8;
    if (tail == 0) return 0;
    return 8 - tail;
}

static unsigned tag_payload(const struct multiboot_header_tag* tag) {
    return (unsigned)(tag->size - sizeof(struct multiboot_header_tag));
}

static const struct multiboot_header* mb_find_header(const void* data, unsigned size) {
    const void* current = data;
    const void* end = data + MULTIBOOT_SEARCH;
    while (current < end) {
        const struct multiboot_header* H = (struct multiboot_header*)current;
        if (H->magic == MULTIBOOT2_HEADER_MAGIC) {
            printf("found magic at offset 0x%x\n", current - data);
            uint32_t sum = H->magic + H->architecture + H->header_length + H->checksum;
            if (sum == 0) return H;
        }
        current += MULTIBOOT_HEADER_ALIGN;
    }
    return NULL;
}

static int multiboot_parse(const void* data, unsigned size)
{
    // TODO sanity check for size
    const struct multiboot_header* H = mb_find_header(data, size);
    if (!H) {
        printf("No multiboot2 header found\n");
        return -1;
    }
    printf("total size %u\n", size);
    printf("[%04x] "ANSI_RED "HEADER"ANSI_NORMAL "  arch %u  header_len %u\n", (unsigned)((const void*)H - data), H->architecture, H->header_length);

    const void* cur = (const void*)&H[1];
    const void* end = (const void*)H + H->header_length;
    while (cur < end) {
        const struct multiboot_header_tag* tag = (const struct multiboot_header_tag*)cur;
        unsigned offset = cur - data;
        printf("[%04x] "ANSI_RED "TAG" ANSI_NORMAL " %2u "ANSI_YELLOW "%12s"ANSI_NORMAL "  flags 0x%04x  size %u\n",
            offset, tag->type, headertag2str(tag->type), tag->flags, tag->size);

        switch (tag->type) {
        case MULTIBOOT_HEADER_TAG_END:
            break;
        case MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST:
        {
            const struct multiboot_header_tag_information_request* info = (const struct multiboot_header_tag_information_request*)tag;
            unsigned payload = tag_payload(tag);
            printf("   requests ");
            for (unsigned i=0; i<payload/sizeof(uint32_t); i++) {
                printf("  %u "ANSI_GREEN "%s"ANSI_NORMAL , info->requests[i], tag2str(info->requests[i]));
            }
            printf("\n");
            break;
        }
        case MULTIBOOT_HEADER_TAG_ADDRESS:
            printf("   unhandled tag\n");
            break;
        case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS:
        {
            const struct multiboot_header_tag_entry_address* entry = (const struct multiboot_header_tag_entry_address*)tag;
            printf("   addr 0x%x\n", entry->entry_addr);
            break;
        }
        case MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS:
            printf("   unhandled tag\n");
            break;
        case MULTIBOOT_HEADER_TAG_FRAMEBUFFER:
        {
            const struct multiboot_header_tag_framebuffer* fb = (const struct multiboot_header_tag_framebuffer*)tag;
            printf("   width %u  height %u  depth %u\n", fb->width, fb->height, fb->depth);
            break;
        }
        case MULTIBOOT_HEADER_TAG_MODULE_ALIGN:
        case MULTIBOOT_HEADER_TAG_EFI_BS:
        case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32:
        case MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64:
        case MULTIBOOT_HEADER_TAG_RELOCATABLE:
            printf("   unhandled tag\n");
            break;
        default:
            printf("   unknown tag\n");
            break;
        }
        unsigned tagsize = tag->size + tag_padding(tag->size);
        cur += tagsize;
        if (tagsize == 0) {
            printf("invalid tagsize 0\n");
            return -1;
        }
    }

    // TODO check grub: grub_multiboot2_make_mbi()
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

    multiboot_parse(map, size);

    munmap(map, size);
    return 0;
}

