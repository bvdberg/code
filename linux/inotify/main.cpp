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
    const struct maskname* name = names;
    while (name->mask != 0) {
        if (name->mask == mask) return name->name;
        name++;
    }
    return "unknown";
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
    printf("wd = %d\n", wd);

    const int buf_len = 10*sizeof(inotify_event);
    char* buf = (char*)malloc(buf_len);

    size_t len = read(fd, buf, buf_len);
    int eventCount = len /  sizeof(inotify_event);
    printf("len = %d (%d events)\n", len, eventCount);

    struct inotify_event* event = (struct inotify_event*) buf;
    for (int i=0; i<eventCount; i++) {
        printf("wd=%d, mask=%u (%s), cookie=%u, len=%u, name=[", event->wd, event->mask, mask2name(event->mask), event->cookie, event->len);
        if (event->len > 0) printf("%s", &event->name[0]);
        printf("]\n");
        event++;
    }

    free(buf);

    return 0;
}

