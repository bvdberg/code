#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>


int main(int argc, const char *argv[])
{
    sync();
    if (reboot(LINUX_REBOOT_CMD_RESTART) == -1) {
        perror("reboot");
    }
    return 0;
}

