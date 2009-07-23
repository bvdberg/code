#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>	/* dlopen(), dlclose(), dlsym() ... */

/* this function prints a usage message and exits. */
void
usage(int argc, char* argv[])
{
    fprintf(stderr, "Usage: %s [1|2]\n", argv[0]);
    exit(1);
}

int main(int argc, char* argv[])
{
    int   lib_num;		/* which library to use?                 */
    char  lib_name[100];	/* contains the name of the library file */
    void* lib_handle;		/* handle for the loaded shared library  */
    void  (*lib_func)();	/* pointer to function in the library    */
    const char* error;		/* pointer to an error message, if any   */

    /* check command line arguments */
    if (argc != 2)
	usage(argc, argv);  /* DOES NOT RETURN */

    /* check which library to load. */
    lib_num = atoi(argv[1]); /* atoi - Array TO Integer conversion. */

    if (lib_num < 1 || lib_num > 2)
	usage(argc, argv);  /* DOES NOT RETURN */

    /* prepare the library's name in a buffer */
    sprintf(lib_name, "./lib%d.so", lib_num);

    /* load the desired shared library */
    lib_handle = dlopen(lib_name, RTLD_LAZY);
    if (!lib_handle) {
	fprintf(stderr, "Error: %s\n", dlerror());
	exit(1);
    }

    /* locate the symbol in the library */
    lib_func = dlsym(lib_handle, "say_something_nice");
    error = dlerror();
    if (error) {
	fprintf(stderr, "Error: %s\n", error);
	exit(1);
    }

    /* call the library's utility function. */
    (*lib_func)();

    /* finally, close the library. */
    dlclose(lib_handle);

    return 0;
}
