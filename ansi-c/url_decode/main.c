#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ANSI_BLACK    "\033[0;30m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_CYAN     "\033[0;36m"
#define ANSI_GREY     "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED     "\033[01;31m"
#define ANSI_BGREEN   "\033[01;32m"
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_BBLUE    "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN    "\033[01;36m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"

#define MAX_URL_LEN 150

typedef struct {
    char* proto;
    char* username;
    char* password;
    char* hostname;
    char* port;
    char* path;
    char* filename;
} url_t;

static bool has_file = true;

static char *rev_index(char *s, char c)
{
    char* res = NULL;
    while (*s) {
        if (*s == c) res = s;
        s++;
    }
    return res;
}

// input should be like "ftp://username:password@hostname:port/path/file"
static bool parse_url(url_t* url, char* input, bool expect_file) {
    memset(url, 0, sizeof(url_t));
    char* cp = input;

    url->proto = cp;
    cp = strstr(cp, "://");
    if (cp) {
        *cp = 0;
        cp += 3;
    } else {
        cp = url->proto;
        url->proto = NULL;
    }

    url->username = cp;
    cp = strstr(cp, "@");
    if (cp) {
        *cp = 0;
        cp++;
        char* pass = strstr(url->username, ":");
        if (pass) {
            *pass = 0;
            pass++;
            url->password = pass;
        }
    } else {
        cp = url->username;
        url->username = NULL;
        url->password = NULL;
    }

    url->hostname = cp;
    cp = strstr(cp, ":");
    if (cp) {
        *cp = 0;
        cp++;
        url->port = cp;
    } else {
        cp = url->hostname;
    }

    url->path = cp;
    cp = strstr(cp, "/");
    if (!cp) return false;

    *cp = 0;
    cp++;
    url->path = cp;

    if (!expect_file) {
        if (url->path[0] == 0) return false;
        return true;
    }

    char* file = rev_index(cp, '/');
    if  (file) {
        *file = 0;
        file++;
        url->filename = file;
    } else {
        url->filename = url->path;
        url->path = NULL;
    }
    if (url->filename[0] == 0) return false;
    return true;
}

static void print_url(const url_t* url) {
    printf("proto:    '%s'\n", url->proto);
    printf("username: '%s'\n", url->username);
    printf("password: '%s'\n", url->password);
    printf("hostname: '%s'\n", url->hostname);
    printf("port:     '%s'\n", url->port);
    printf("path:     '%s'\n", url->path);
    printf("filename: '%s'\n", url->filename);
}

static void test(const char* input) {
    printf("%sparsing [%s]%s\n", ANSI_YELLOW, input, ANSI_NORMAL);
    fflush(stdout);
    char str[MAX_URL_LEN];
    strcpy(str, input);
    url_t url;
    bool ok = parse_url(&url, str, has_file);
    if (!ok) {
        printf("%sinvalid url%s\n", ANSI_RED, ANSI_NORMAL);
    }
    print_url(&url);
}

int main(int argc, const char *argv[])
{
    test("ftp://user:pass@hostname:21/foo/bar/faa.bin");
    test("hostname:21/foo/bar/faa.bin");
    test("hostname/file.bin");
    test("hostname/path/file.bin");
    test("user@hostname/file.bin");
    test("user@hostname/");
    test("ftp://ecotap:ecotap123@192.168.178.18:21/upload");
    return 0;
}

