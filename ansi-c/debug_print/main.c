#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_NORMAL   "\033[0m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define LOG(fmt, args...); fprintf(stderr, ANSI_BYELLOW "LOG %s() " fmt "\n" ANSI_NORMAL, __func__, ##args);
#else
#define LOG(fmt, args...);
#endif


int main(int argc, const char *argv[])
{
    LOG("before hello world");
    printf("Hello world\n");
    LOG("after %d", 10);
    return 0;
}

