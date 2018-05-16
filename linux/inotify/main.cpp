/*
 * compile and run: ./a.out /tmp
 *
 * now touch or delete a file in /tmp
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/inotify.h>

int stop = 0;

static void sigint(int) {
    stop = true;
}

struct maskname {
    uint32_t mask;
    const char* name;
};

const struct maskname names[] = {
    { IN_CREATE, "file created" },
    { IN_DELETE, "file deleted" },
    { IN_CREATE | IN_ISDIR, "dir created" },
    { IN_CREATE | IN_ISDIR, "dir deleted" },
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
    signal(SIGINT, sigint);

    int fd = inotify_init();
    printf("fd=%d\n", fd);

    printf("watching %s\n", argv[1]);
    int wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("inotify_add_watch");
        return -1;
    }
    printf("wd = %d\n", wd);

    while (!stop) {
        const int buf_len = 1024;
        char* buf = (char*)malloc(buf_len);

        ssize_t len = read(fd, buf, buf_len);
        if (len < 0) {
            perror("read");
            free(buf);
            break;
        }
        printf("len = %d\n", (int)len);
        struct inotify_event* event = (struct inotify_event*) buf;
        int i = 0;
        while (i < len) {
            if (event->wd != wd) {
                printf("invalid wd: %d\n", event->wd);
                return -1;
            }
            printf("wd=%d, mask=0x%x (%s), cookie=%u, len=%u, name=[", event->wd, event->mask, mask2name(event->mask), event->cookie, event->len);
            if (event->len > 0) printf("%s", &event->name[0]);
            printf("]\n");
            i += (sizeof(inotify_event) + event->len);
            event++;
        }

        free(buf);
    }
    int ret = inotify_rm_watch (fd, wd);
    if (ret) perror ("inotify_rm_watch");

    return 0;
}

