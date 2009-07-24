#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>

void bt_sighandler(int sig, struct sigcontext ctx) {

    void *trace[16];
    char **messages = (char **)NULL;
    int i, trace_size = 0;

    if (sig == SIGSEGV)
        printf("Got signal %d, faulty address is %p, "
               "from %p\n", sig, ctx.cr2, ctx.eip);
    else
        printf("Got signal %d\n", sig);

    trace_size = backtrace(trace, 16);
    /* overwrite sigaction with caller's address */
    trace[1] = (void *)ctx.eip;
    messages = backtrace_symbols(trace, trace_size);
    /* skip first stack frame (points here) */
    printf("[bt] Execution path:\n");
    for (i=1; i<trace_size; ++i)
        printf("[bt] %s\n", messages[i]);

    exit(0);
}


int func_a(int a, char b) {

    char *p = (char *)0xdeadbeef;

    a = a + b;
    *p = 10;  /* CRASH here!! */

    return 2*a;
}


int func_b() {

    int res, a = 5;

    res = 5 + func_a(a, 't');

    return res;
}


int main() {

    /* Install our signal handler */
    struct sigaction sa;

    sa.sa_handler = (void *)bt_sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    /* ... add any other signal here */

    /* Do something */
    printf("%d\n", func_b());


}


