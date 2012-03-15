#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ubi-media.h"

#define BLOCKSIZE 128*1024
#define PAGE_SIZE 4096
// for volume_id blocks?
#define RESERVED_BLOCKS 4

#define be64_to_cpu(x) __swab64((x))

/*
    + flashing to file: args: input file, num_blocks, output_file
    + check input file for correctness
        -> check if size multiple of num_blocks
    + also check types (dynamic not static, etc) -> SKIP
    + check of aantal blocks genoeg is (reserveer 4 voor VOLUME_ID)
    + malloc totale grootte + init to zero (output_map)
    - schrijf elke blok uit volume 0 naar zelfde blok in output file
    - creeer 2 nieuwe volumeID blocken met nieue reserved_pebs en crc
    - vul resterende blokken op met default block (hardcoded values including crc)
        en 0xff als data fill
    - open output file (deleting existing one
    - write map to file
    - close output file
*/

enum Mode { MODE_SHOW=0, MODE_FLASH };
enum Mode mode = MODE_SHOW;

struct ubi_info {
    int num_erase_blocks;
    int num_data_blocks;
    int num_volume_blocks;
};


static int parse_ubi(void* input_map, int size, struct ubi_info* info) {
    unsigned char* base = input_map;
    int offset = 0;
    int index = 0;
    int vid_count = 0;
    int vtbl_count = 0;
    while (offset < size) {
        struct ubi_ec_hdr* hdr = (struct ubi_ec_hdr*)(base + offset);
        if (ntohl(hdr->magic) != UBI_EC_HDR_MAGIC) {
            printf("[%03d] [%08d] invalid magic\n", index, offset);
            return 1;
        }
        unsigned long ec = be64_to_cpu(hdr->ec);
        unsigned int vid = ntohl(hdr->vid_hdr_offset);
        unsigned int data = ntohl(hdr->data_offset);
        unsigned int crc = ntohl(hdr->hdr_crc);
        printf("[%03d] [%08x]  vid=%u  daa=%u  crc=0x%08x  ec=%ld\n", index, offset, vid, data, crc, ec);

        struct ubi_vid_hdr* vid_hdr = (struct ubi_vid_hdr*)&hdr[1];
        if (ntohl(vid_hdr->magic) == UBI_VID_HDR_MAGIC) {
            unsigned int vol_id = ntohl(vid_hdr->vol_id);
            unsigned int lnum = ntohl(vid_hdr->lnum);
            unsigned int datasize = ntohl(vid_hdr->data_size);
            unsigned int used_ebs = ntohl(vid_hdr->used_ebs);
            unsigned int data_pad = ntohl(vid_hdr->data_pad);
            unsigned int data_crc = ntohl(vid_hdr->data_crc);
            unsigned int sqnum = ntohl(vid_hdr->sqnum);
            unsigned int hdr_crc2 = ntohl(vid_hdr->hdr_crc);
            printf("  VID_HDR:  type=%d  copy=%d  vol_id=0x%08x  lnum=%d  datasize=%d\n",
                vid_hdr->vol_type, vid_hdr->copy_flag, vol_id, lnum, datasize);
            printf("  used_ebs=%d  data_pad=%d  crc=0x%08x  sqnum=%d  hdr_crc=0x%08x\n",
                used_ebs, data_pad, data_crc, sqnum, hdr_crc2);
            vid_count++;

            if (vol_id == UBI_LAYOUT_VOLUME_ID) {
                struct ubi_vtbl_record* vtbl = (struct ubi_vtbl_record*)&vid_hdr[1];
                unsigned int res_pebs = ntohl(vtbl->reserved_pebs);
                unsigned int alignment = ntohl(vtbl->alignment);
                unsigned int data_pad = ntohl(vtbl->data_pad);
                unsigned int name_len = ntohs(vtbl->name_len);
                unsigned int crc = ntohl(vtbl->crc);
                printf("    VOLUME_ID: res_peds=%d  align=%d  data_pad=%d  type=%d  marker=%d\n",
                    res_pebs, alignment, data_pad, vtbl->vol_type, vtbl->upd_marker);
                printf("    len=%d  name=%s  flags=%d  crc=0x%08x\n", name_len, vtbl->name, vtbl->flags, crc);
                vtbl_count++;
            }
        }

        offset += BLOCKSIZE;
        index++;
    }
    if (size != BLOCKSIZE * index) {
        printf("Invalid size, expected %d, got %d\n", BLOCKSIZE * index, size);
        return 1;
    }
    printf("%d PEBs,  %d VIDs  %d VTBLs\n", index, vid_count, vtbl_count);
    printf("SIZEOF ec_hdr=%d  vid_hdr=%d  vtbl=%d\n",
        sizeof(struct ubi_ec_hdr), sizeof(struct ubi_vid_hdr), sizeof(struct ubi_vtbl_record));
    if (info) {
        info->num_erase_blocks = index;
        info->num_data_blocks = vid_count - vtbl_count;
        info->num_volume_blocks = vtbl_count;
    }
    return 0;
}


static int copy_volume(void* input_map, int input_size, void* output_map, unsigned int volumeID) {
    int output_index = 0;

    unsigned char* base = input_map;
    int offset = 0;
    int input_index = 0;
    while (offset < input_size) {
        struct ubi_ec_hdr* hdr = (struct ubi_ec_hdr*)(base + offset);
        if (ntohl(hdr->magic) != UBI_EC_HDR_MAGIC) {
            printf("[%03d] [%08d] invalid magic\n", input_index, offset);
            return 0;
        }
        struct ubi_vid_hdr* vid_hdr = (struct ubi_vid_hdr*)&hdr[1];
        if (ntohl(vid_hdr->magic) == UBI_VID_HDR_MAGIC) {
            unsigned int vol_id = ntohl(vid_hdr->vol_id);
            unsigned int lnum = ntohl(vid_hdr->lnum);
            if (vol_id == volumeID) {
                void* input_ptr = (void*)(base + offset);
                void* output_ptr = output_map + (output_index * BLOCKSIZE);
                printf("copying block %d to block %d,  index %d  0x%08x -> 0x%08x\n", input_index, output_index, lnum, (unsigned int)input_ptr, (unsigned int)output_ptr);
                memcpy(output_ptr, input_ptr, BLOCKSIZE);
                // copy block to output
                output_index++;
            }
        }

        offset += BLOCKSIZE;
        input_index++;
    }
    return output_index;
}


static void mark_empty_blocks(void* output_map, int output_index, int num_blocks) {
    struct ubi_ec_hdr ec_hdr;
    memset(&ec_hdr, 0, sizeof(struct ubi_ec_hdr));
    ec_hdr.magic = cpu_to_be32(UBI_EC_HDR_MAGIC);
    ec_hdr.version = UBI_VERSION;
    ec_hdr.ec = cpu_to_be64(3); // to match crc
    ec_hdr.vid_hdr_offset = cpu_to_be32(64);
    ec_hdr.data_offset = cpu_to_be32(128);
    ec_hdr.hdr_crc = cpu_to_be32(0xf55dc15a);


    while (output_index < num_blocks) {
        void* output_ptr = output_map + (output_index * BLOCKSIZE);
        memset(output_ptr, 0xff, BLOCKSIZE);
        memcpy(output_ptr, &ec_hdr, UBI_EC_HDR_SIZE);
        output_index++;
    }
}


static void flash_ubi(void* input_map, int size, const char* output_file, int num_blocks) {
    struct ubi_info info;
    int err = parse_ubi(input_map, size, &info);
    if (err) return;
    printf("INFO: %d blocks [%d data, %d vol]\n", info.num_erase_blocks, info.num_data_blocks, info.num_volume_blocks);
    // Q: check for empty blocks?
    // check if output size is sufficient
    int needed_blocks = info.num_data_blocks + RESERVED_BLOCKS;
    if (num_blocks < needed_blocks) {
        printf("Not enough size, got %d blocks, need %d blocks\n", num_blocks, needed_blocks);
        return;
    }

    int output_size = num_blocks * BLOCKSIZE;
    printf("output file %s,  size %d\n", output_file, output_size);
    void* output_map = calloc(num_blocks, BLOCKSIZE);
    if (output_map == NULL) {
        perror("calloc");
        return;
    }
    // write Volume0 blocks to output_map
    int output_index = copy_volume(input_map, size, output_map, 0);
    printf("copied %d blocks\n", output_index);

    // copy volume table entries, and resize for output size, recalc crc, etc

    // mark remaining blocks as empty
    mark_empty_blocks(output_map, output_index, num_blocks);

    // open output file
    int out_fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0660);
    if (out_fd == -1) {
        perror("open");
        goto out;
    }

    // write output_map to output_file
    int pages = output_size / PAGE_SIZE;
    int i;
    for (i=0; i<pages; i++) {
        int written = write(out_fd, output_map + i * PAGE_SIZE, PAGE_SIZE);
        if (written != PAGE_SIZE) {
            perror("write");
            break;
        }
    }
    close (out_fd);
out:
    free (output_map);
}

        
int main(int argc, const char *argv[]) {
    const char* input_file = NULL;
    const char* output_file = 0;
    int num_blocks = 0;

    switch (argc) {
    case 2:
        mode = MODE_SHOW;
        input_file = argv[1];
        break;
    case 4:
        mode = MODE_FLASH;
        input_file = argv[1];
        num_blocks = atoi(argv[2]);
        output_file = argv[3];
        break;
    default:
        printf("Usage %s [input_file] <numblocks> <output_file>\n", argv[0]);
        return -1;
    }

    int fd = open(input_file, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) != 0) {
        perror("fstat");
        return -1;
    }

    int size = statbuf.st_size;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    void* input_map = mmap(NULL, size, prot, flags, fd, 0);
    if (input_map == (void*)-1) {
        perror("mmap");
        return -1;
    }
    close(fd);

    switch (mode) {
    case MODE_SHOW:
        parse_ubi(input_map, size, NULL);
        break;
    case MODE_FLASH:
        flash_ubi(input_map, size, output_file, num_blocks);
        break;
    }
/*
    unsigned char* base = input_map;
    int offset = 0;
    int index = 0;
    int vid_count = 0;
    int vtbl_count = 0;
    while (offset < size) {
        struct ubi_ec_hdr* hdr = (struct ubi_ec_hdr*)(base + offset);
        if (ntohl(hdr->magic) != UBI_EC_HDR_MAGIC) {
            printf("[%03d] [%08d] invalid magic\n", index, offset);
            break;
        }
        unsigned long ec = be64_to_cpu(hdr->ec);
        unsigned int vid = ntohl(hdr->vid_hdr_offset);
        unsigned int data = ntohl(hdr->data_offset);
        unsigned int crc = ntohl(hdr->hdr_crc);
        printf("[%03d] [%08x]  vid=%u  daa=%u  crc=0x%08x  ec=%ld\n", index, offset, vid, data, crc, ec);

        struct ubi_vid_hdr* vid_hdr = (struct ubi_vid_hdr*)&hdr[1];
        if (ntohl(vid_hdr->magic) == UBI_VID_HDR_MAGIC) {
            unsigned int vol_id = ntohl(vid_hdr->vol_id);
            unsigned int lnum = ntohl(vid_hdr->lnum);
            unsigned int datasize = ntohl(vid_hdr->data_size);
            unsigned int used_ebs = ntohl(vid_hdr->used_ebs);
            unsigned int data_pad = ntohl(vid_hdr->data_pad);
            unsigned int data_crc = ntohl(vid_hdr->data_crc);
            unsigned int sqnum = ntohl(vid_hdr->sqnum);
            unsigned int hdr_crc2 = ntohl(vid_hdr->hdr_crc);
            printf("  VID_HDR:  type=%d  copy=%d  vol_id=0x%08x  lnum=%d  datasize=%d\n",
                vid_hdr->vol_type, vid_hdr->copy_flag, vol_id, lnum, datasize);
            printf("  used_ebs=%d  data_pad=%d  crc=0x%08x  sqnum=%d  hdr_crc=0x%08x\n",
                used_ebs, data_pad, data_crc, sqnum, hdr_crc2);
            vid_count++;

            if (vol_id == UBI_LAYOUT_VOLUME_ID) {
                struct ubi_vtbl_record* vtbl = (struct ubi_vtbl_record*)&vid_hdr[1];
                unsigned int res_pebs = ntohl(vtbl->reserved_pebs);
                unsigned int alignment = ntohl(vtbl->alignment);
                unsigned int data_pad = ntohl(vtbl->data_pad);
                unsigned int name_len = ntohs(vtbl->name_len);
                unsigned int crc = ntohl(vtbl->crc);
                printf("    VOLUME_ID: res_peds=%d  align=%d  data_pad=%d  type=%d  marker=%d\n",
                    res_pebs, alignment, data_pad, vtbl->vol_type, vtbl->upd_marker);
                printf("    len=%d  name=%s  flags=%d  crc=0x%08x\n", name_len, vtbl->name, vtbl->flags, crc);
                vtbl_count++;
            }
        }

        offset += BLOCKSIZE;
        index++;
    }
    printf("%d PEBs,  %d VIDs  %d VTBLs\n", index, vid_count, vtbl_count);
    printf("SIZEOF ec_hdr=%d  vid_hdr=%d  vtbl=%d\n",
        sizeof(struct ubi_ec_hdr), sizeof(struct ubi_vid_hdr), sizeof(struct ubi_vtbl_record));
*/

    munmap(input_map, size);
    return 0;
}

