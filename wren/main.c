#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "wren/wren.h"

typedef struct {
    void* map;
    uint32_t size;
} File;


static File file_open(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        close(fd);
        exit(EXIT_FAILURE);
    }
    unsigned int size = statbuf.st_size;

    void* map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    close(fd);
    File file = { .map = map, .size = size };
    return file;
}

static void file_close(File f) {
    munmap(f.map, f.size);
}

static const char* file_data(File f) {
    return (const char*)f.map;
}

static void writeFn(WrenVM* vm, const char* text)
{
  printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
    switch (errorType) {
    case WREN_ERROR_COMPILE:
        printf("[%s line %d] [Error] %s\n", module, line, msg);
        break;
    case WREN_ERROR_STACK_TRACE:
        printf("[%s line %d] in %s\n", module, line, msg);
        break;
    case WREN_ERROR_RUNTIME:
        printf("[Runtime Error] %s\n", msg);
        break;
    }
}

int main(int argc, const char *argv[])
{
if (argc != 2) {
    printf("Usage: %s [file]\n", argv[0]);
    return -1;
}
const char* filename = argv[1];
File file = file_open(filename);

WrenConfiguration config = {};
wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
    WrenVM* vm = wrenNewVM(&config);

    const char* module = "main";
    WrenInterpretResult res = wrenInterpret(vm, module, file_data(file));
    switch (res) {
    case WREN_RESULT_COMPILE_ERROR:
        printf("wren: Compile Error\n");
        break;
    case WREN_RESULT_RUNTIME_ERROR:
        printf("wren: Runtime Error\n");
        break;
    case WREN_RESULT_SUCCESS:
        printf("wren: Success\n");
        break;
    }

    WrenHandle* handle = wrenMakeCallHandle(vm, "todo");
    printf("handle %p\n", handle);

    if (handle) {
        res = wrenCall(vm, handle);
        if (res != WREN_RESULT_SUCCESS) {
            // error fn will be called already
        }
        wrenReleaseHandle(vm, handle);
    }

    wrenFreeVM(vm);
    file_close(file);
    return 0;
}

