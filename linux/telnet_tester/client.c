#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ANSI_BLACK "\033[0;30m"
#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_GREY "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED "\033[01;31m"
#define ANSI_BGREEN "\033[01;32m"
#define ANSI_BYELLOW "\033[01;33m"
#define ANSI_BBLUE "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_NORMAL "\033[0m"

/*
    - timestamp all send/receives
    - also print delay
*/

#define IAC 255     //telnet symbol

static int sockfd = 0;

static const char* name = 0;
static const char* passwd = 0;

static int verbose = 1;
static int logtime = 1;

static u_int64_t sendTime = 0;
static u_int64_t recvTime = 0;
static u_int64_t startTime = 0;
static u_int64_t cmd_timeout_usec = 5000000;

static const char* printTime(u_int64_t now)
{
    static char buffer[32];
    u_int64_t subsec = now % 1000000lu;
    sprintf(buffer, "%04" PRIu64".%06" PRIu64, now / 1000000lu, subsec);
    return buffer;
}

static u_int64_t getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec;
    if (startTime == 0) {
        startTime = now64;
    }
    now64 -= startTime;
    now64 *= 1000000;
    now64 += (now.tv_usec);
    return now64;
}

#if 0
#define LF 10
#define CR 13
static int stripNewline(char* buf) {
    int len = strlen(buf);
    if (buf[len-1] == LF) buf[len-1] = 0;
    if (buf[len-2] == LF) buf[len-2] = 0;
    if (buf[len-1] == CR) buf[len-1] = 0;
    if (buf[len-2] == CR) buf[len-2] = 0;
    return 0;
}

static int endsWith(const char* string, const char* tail) {
    int len1 = strlen(string);
    int len2 = strlen(tail);
    if (len1 < len2) return 0;

    int start = len1 - len2;
    return (strcmp(string+start, tail) == 0);
}

static char* removeTelnet(unsigned char* buffer) {
    unsigned char* cp = buffer;
    int len = strlen((char*)buffer);
    // remove from start
    while (*cp == IAC) {
        if (cp+3 > buffer+len) {
           *cp = 0;
           return (char*)cp;
        }
        cp += 3; // skip telnet
    }

    // remove from tail
    unsigned char* ep = cp;
    while (*ep) {
        if (*ep == IAC) *ep = 0;
        else ep++;
    }
    return (char*)cp;
}
#endif

static int waitfor(const char* expected) {
    u_int64_t now = getCurrentTime();
    printf("[%s] expecting <"ANSI_GREEN"%s"ANSI_NORMAL">\n", printTime(now), expected);
    u_int64_t stopTime = now + cmd_timeout_usec;
    while (now < stopTime) {
        struct timeval timeout;
        u_int64_t delay = stopTime - now;
        timeout.tv_sec = (delay / 1000000lu);
        timeout.tv_usec = (delay % 1000000lu);
        fd_set rdfs;
        FD_SET(sockfd, &rdfs);

        int res = select(sockfd+1, &rdfs, NULL, NULL, &timeout);
        recvTime = getCurrentTime();
        if (res < 0) {
            printf("ERROR on recv\n");
            perror("recv");
            return 1;
        }
        if (res > 0) {
            unsigned char buffer[4096];
            memset(buffer, 0, sizeof(buffer));
            int recvbytes = recv(sockfd, &buffer, sizeof(buffer), 0);
            if (recvbytes == -1 || recvbytes == 0) {
                printf("%s() RECV ERROR\n", __func__);
                return 1;
            }
            buffer[recvbytes] = 0;
            unsigned offset = 0;
            // NOTE: a buffer can contain multiple replies
            if (logtime && verbose) printf(ANSI_BLUE"[%s] received %d bytes"ANSI_NORMAL"\n", printTime(recvTime), recvbytes);
            while (offset < (unsigned)recvbytes) {
                const char* reply = (char*)&buffer[offset];
                size_t len = strlen(reply);
                //printf(ANSI_BMAGENTA"--- Reply [offset=%d, len=%d] ---"ANSI_NORMAL"\n", offset, len);
                printf("%s\n", reply);
                offset += (len + 1);

                const char* match = strstr(reply, expected);
                if (match) return 0;
            }
        }
        now = getCurrentTime();
    }
    printf(ANSI_BRED"[%s] TIMEOUT on '%s'"ANSI_NORMAL"\n", printTime(now), expected);
    return 1;
}


static void mysend(const char* cmd) {
    sendTime = getCurrentTime();
    char buffer[128];
    sprintf(buffer, "%s\n", cmd);
    if (verbose) {
        printf("[%s] send: <"ANSI_BYELLOW"%s"ANSI_NORMAL">\n", printTime(sendTime), cmd);
    }
	int len = strlen(buffer);
    int written = write(sockfd, buffer, len);
	if (len != written) fprintf(stderr, "error writing '%s'\n", buffer);
}


static int handleLine(int line, const char* buffer, int* cmd_count) {
    int len = strlen(buffer);
    if (len < 3) return 0;
    if (len > 255) {
        printf("line too long at line %d\n", line);
        return 1;
    }
    if (buffer[0] == '|') return 0; // comments
    if (buffer[1] != '(' || buffer[len-1] != ')') {
        printf("invalid syntax at line %d: '%s'\n", line, buffer);
        return 1;
    }
    char text[255];
    strcpy(text, buffer+2);
    text[strlen(text)-1] = 0;

    char cmd = buffer[0];
    switch (cmd) {
    case 's':       // send
        mysend(text);
        (*cmd_count)++;
        return 0;
    case 'w':       // sleep
        usleep(1000*atoi(text));
        return 0;
    case 'u':       // until
        return waitfor(text);
    case 'n':       // name
        if (!name) {
            printf("name not set in args!\n");
            return 1;
        }
        mysend(name);
        return 0;
    case 'p':       // password
        if (!passwd) {
            printf("passwd net set in args!\n");
            return 1;
        }
        mysend(passwd);
        return 0;
    default:
        printf("unknown command '%c' at line %d\n", cmd, line);
        return 1;
    }
}


#define LF 10
#define CR 13
int main(int argc, char* argv[]) {
    int port = 2323;
    const char* dest = "127.0.0.1";

    if (argc < 2) {
        printf("Usage: test [file] <dest> <port>\n");
        return -1;
    }
    if (argc >= 3) dest = argv[2];
    if (argc >= 4) port = atoi(argv[3]);

    const char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == 0) {
        perror("fopen");
        return -1;
    }

    srandom(time(0));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("error opening socket\n");
        fclose(file);
        return -1;
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(dest);
    memset(&(dest_addr.sin_zero), '\0', 8);

    sendTime = getCurrentTime();
    int res = connect(sockfd, (struct sockaddr* )&dest_addr, sizeof(struct sockaddr));
    if (res == -1) {
        printf("error connecting to %s:%d\n", dest, port);
        fclose(file);
        return -1;
    }
    printf(ANSI_YELLOW"connected to %s:%d"ANSI_NORMAL"\n", dest, port);

    char* buffer = 0;
    size_t bufsize = 0;
    int line = 1;

    u_int64_t t_begin = getCurrentTime();
    int cmd_count = 0;
    while (1) {
        ssize_t size = getline(&buffer, &bufsize, file);
        if (size == -1) break;
        if (buffer[size-2] == CR) buffer[size-2] = 0;
        if (buffer[size-1] == LF) buffer[size-1] = 0;
        int err = handleLine(line, buffer, &cmd_count);
        if (err) break;
        ++line;
    }
    u_int64_t t_end = getCurrentTime();
    free(buffer);
    close(sockfd);

    float cmd_per_sec = cmd_count / ((t_end - t_begin) / 1000000.0);
    printf(ANSI_BLUE"done"ANSI_NORMAL"  %s (%d cmds, %.1f/sec) took %" PRIu64" ms\n", filename, cmd_count, cmd_per_sec, (t_end-t_begin)/1000);
    return 0;
}

