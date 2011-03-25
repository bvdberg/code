
#include <stdio.h>


struct cmd_tbl_s {
    char    *name;    /* Command Name     */
    int   maxargs;  /* maximum number of arguments  */
    int   repeatable; /* autorepeat allowed?    */

    int   (*cmd)(struct cmd_tbl_s *, int, int, char *[]);
    char    *usage;   /* Usage message  (short) */
};

typedef struct cmd_tbl_s  cmd_tbl_t;


extern cmd_tbl_t __boot_cmd_start;
extern cmd_tbl_t __boot_cmd_end;

cmd_tbl_t __u_boot_cmd_bas  __attribute__ ((unused,section (".u_boot_cmd"))) = { "basbas", 0, 0, 0, 0};
cmd_tbl_t __u_boot_cmd_bas1  __attribute__ ((unused,section (".u_boot_cmd"))) = { "basbas1", 0, 0, 0, 0};




int main()
{
    cmd_tbl_t* cmd;
    printf("%p\n", &__boot_cmd_start);
    printf("%p\n", &__boot_cmd_end);
    printf("%s\n", (char*)&__boot_cmd_start);

    for (cmd = &__boot_cmd_start; cmd != &__boot_cmd_end; cmd++) {
        printf("cmd = %p  name=%s\n", cmd, cmd->name);
    }
    return 0;
}


