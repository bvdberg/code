#include <stdio.h>
#include <string.h>

#define TEXT "Foo Bar Faa"
#define TEXT2 "The quick brown fox"

#define str(x) #x
#define glue(a,b) a ## b

int main(int argc, const char *argv[])
{
    printf("%s\n", TEXT);
    printf("%s\n", str(TEXT));
    glue(pri, ntf)("BLA BLA\n");

    return 0;
}
