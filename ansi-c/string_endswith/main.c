#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int endsWith(const char* text, const char* tail)
{
    int text_len = strlen(text);
    int tail_len = strlen(tail);
    if (tail_len > text_len) return 0;

    return (strcmp(text+text_len-tail_len, tail) == 0);
}


void test(const char* text, const char* tail, int expected) {
    int res = endsWith(text, tail);
    if (res != expected) printf("ERROR endsWith(%s, %s) gives %d\n", text, tail, res);
}


int main(int argc, const char *argv[])
{
    test("", "", TRUE); 
    test("abc", "abc", TRUE); 
    test("abcd", "abc", FALSE); 
    test("abcd", "", TRUE); 
    test("name.txt", ".txt", TRUE);

    return 0;
}

