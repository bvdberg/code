#include <stdio.h>

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

int main() {

    char* input = N_("translation");

//    setlocale (LC_CTYPE, "");
//    setlocale (LC_MESSAGES, "");
    bindtextdomain("bastest", "/home/bas/code/achive/linux/gettext");
    textdomain("bastest");

    printf("Text is '%s'\n", input);
    

    return 0;
}

