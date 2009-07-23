
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
    int i = 0;
    while(envp[i] != 0) {
        printf("envp[%d] = [%s]\n", i, envp[i]);
        i++;
    }
    printf("var=%s\n", getenv("test"));
    return 0;
}

