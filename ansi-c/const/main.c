#include <stdio.h>

int main(int argc, const char *argv[])
{
    const char* text1 = "Foo";
    //text1[1] = 'X'; // error, modifying read-only location
    text1 = "Faa";    // ok

    char* const text2 = "Bliep";
    text2[2] = 'o'; // ok, modifying data
    //text2 = "Zaph"; // error, changing read-only variable

    return 0;
}

