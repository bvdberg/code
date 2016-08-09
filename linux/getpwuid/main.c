#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, const char *argv[])
{
    struct passwd* pwd = getpwuid(getuid());

    printf("name = '%s'\n", pwd->pw_name);
    printf("password = '%s'\n", pwd->pw_passwd);
    printf("uid = %d\n", pwd->pw_uid);
    printf("gid = %d\n", pwd->pw_gid);
    printf("gecos = '%s'\n", pwd->pw_gecos);
    printf("dir = '%s'\n", pwd->pw_dir);
    printf("shell = '%s'\n", pwd->pw_shell);
#if 0
       struct passwd {
           char   *pw_name;       /* username */
           char   *pw_passwd;     /* user password */
           uid_t   pw_uid;        /* user ID */
           gid_t   pw_gid;        /* group ID */
           char   *pw_gecos;      /* user information */
           char   *pw_dir;        /* home directory */
           char   *pw_shell;      /* shell program */
       };
#endif

    return 0;
}

