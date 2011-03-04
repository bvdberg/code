/*
 * compile and run: ./a.out /tmp
 *
 * now touch or delete a file in /tmp
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/inotify.h>

struct maskname {
    uint32_t mask;
    const char* name;
};

const struct maskname names[] = {
    {0x100, "file created" },
    {0x200, "file deleted" },
    {0, 0}
};

const char* mask2name(uint32_t mask) {
    static char buffer[20];
    const struct maskname* name = names;
    while (name->mask != 0) {
        if (name->mask == mask) return name->name;
        name++;
    }
    sprintf(buffer, "unknown: 0x%x", mask);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage %s [dir to watch]\n", argv[0]);
        return 1;
    }
    printf("sizeof(inotify_event) = %d\n", sizeof(inotify_event));

    int fd = inotify_init();
    printf("fd=%d\n", fd);

    printf("watching %s\n", argv[1]);
    int wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("inotify_add_watch");
        return -1;
    }
    printf("wd = %d\n", wd);

    while (1) {
        const int buf_len = 10*sizeof(inotify_event);
        char* buf = (char*)malloc(buf_len);

        size_t len = read(fd, buf, buf_len);
        if (len < 0) {
            perror("read");
            free(buf);
            continue;
        }
        printf("len = %d\n", len);
        struct inotify_event* event = (struct inotify_event*) buf;
        unsigned int i = 0;
        while (i < len) {
            if (event->wd != wd) {
                printf("invalid wd: %d\n", event->wd);
                return -1;
            }
            printf("wd=%d, mask=%u (%s), cookie=%u, len=%u, name=[", event->wd, event->mask, mask2name(event->mask), event->cookie, event->len);
            if (event->len > 0) printf("%s", &event->name[0]);
            printf("]\n");
            i += (sizeof(inotify_event) + event->len);
            event++;
        }

        free(buf);
    }

    return 0;
}

