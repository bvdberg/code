#include <stdio.h>
#include <syslog.h>

int main(int argc, const char *argv[])
{
    openlog("my user log", 0, LOG_USER);

    syslog(LOG_ALERT, "my user alert");
    syslog(LOG_CRIT, "my user crit");
    syslog(LOG_ERR, "my user err");
    syslog(LOG_WARNING, "my user warning");
    syslog(LOG_NOTICE, "my user notice");
    syslog(LOG_INFO, "my user info");
    syslog(LOG_DEBUG, "my user debug");

    closelog();

    return 0;
}

