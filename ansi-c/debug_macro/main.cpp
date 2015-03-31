#include <stdio.h>

#define DEBUG 1

#ifdef DEBUG

#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_NORMAL   "\033[0m"
#define LOG(fmt, args...); fprintf(stderr, ANSI_BYELLOW "LOG %s() " fmt "\n" ANSI_NORMAL, __func__, ##args);

#else

#define LOG(fmt, args...);

#endif

void test() {
    LOG();
}

int main(int argc, const char *argv[])
{
    //LOG();
    LOG("test");
    return 0;
}

