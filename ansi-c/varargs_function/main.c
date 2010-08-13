#include <stdio.h>
#include <stdarg.h>


static void run(const char* format, ...) {
    char buffer[1024];
    va_list(Args);
    va_start(Args, format);
    int len = vsprintf(buffer, format, Args);
    va_end(Args);

    printf("STRING = '%s'\n", buffer);
}

int main(int argc, const char *argv[])
{
    run("abc %d %s", 10, "foo"); 
    return 0;
}
