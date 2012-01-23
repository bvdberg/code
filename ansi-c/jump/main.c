#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf env;
static jmp_buf env2;

int function2(char n) {
    printf("function2(%c)\n", n);
    if (n == 'D') longjmp(env, 5);
    if (n == 'F') longjmp(env2, 6);
    printf("returning from function2(%c)\n", n);
    return 15;
}


void function1() {
    char i;
    for (i='A'; i<'H'; i++) {
        printf("----- %c ----\n", i);
        int res = setjmp(env);
        if (res != 0) {
            printf("coming from jump ONE, res=%d\n", res);
            continue;
        }
        res = setjmp(env2);
        if (res != 0) {
            printf("coming from jump TWO, res=%d\n", res);
            continue;
        }
        usleep(250000);

        res = function2(i);
    }
}

int main(int argc, const char *argv[])
{
    function1();
    
    return 0;
}

