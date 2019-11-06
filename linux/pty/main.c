/*
    pseudo-tty example

    source:
    https://stackoverflow.com/questions/476354/how-do-nix-pseudo-terminals-work-whats-the-master-slave-channel

    Modified style + minor changes
*/

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_BSD
#include <termios.h>


int main(void)
{
    int fdm = posix_openpt(O_RDWR);
    if (fdm < 0) {
        fprintf(stderr, "Error %d on posix_openpt()\n", errno);
        return 1;
    }

    int rc = grantpt(fdm);
    if (rc != 0) {
        fprintf(stderr, "Error %d on grantpt()\n", errno);
        return 1;
    }

    rc = unlockpt(fdm);
    if (rc != 0) {
        fprintf(stderr, "Error %d on unlockpt()\n", errno);
        return 1;
    }

    // Open the slave PTY
    int fds = open(ptsname(fdm), O_RDWR);
    printf("Virtual interface configured\n");
    printf("The master side is named : %s\n", ptsname(fdm));

    // Creation of a child process
    if (fork()) { // Parent

        close(fds); // Close the slave side of the PTY

        while (1) {
            char input[150];
            // Operator's entry (standard input = terminal)
            write(1, "Input : ", sizeof("Input : "));
            rc = read(0, input, sizeof(input));
            if (rc > 0) {
                // Send the input to the child process through the PTY
                write(fdm, input, rc);

                // Get the child's answer through the PTY
                rc = read(fdm, input, sizeof(input) - 1);
                if (rc > 0)
                {
                    // Make the answer NUL terminated to display it as a string
                    input[rc] = '\0';

                    fprintf(stderr, "%s", input);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    } else { // Child

        close(fdm); // Close the master side of the PTY

        struct termios slave_orig_term_settings; // Saved terminal settings
        struct termios new_term_settings; // Current terminal settings

        // Save the default parameters of the slave side of the PTY
        rc = tcgetattr(fds, &slave_orig_term_settings);

        // Set raw mode on the slave side of the PTY
        new_term_settings = slave_orig_term_settings;
        cfmakeraw (&new_term_settings);
        tcsetattr (fds, TCSANOW, &new_term_settings);

        // The slave side of the PTY becomes the standard input and outputs of the child process
        close(0); // Close standard input (current terminal)
        close(1); // Close standard output (current terminal)
        close(2); // Close standard error (current terminal)

        dup(fds); // PTY becomes standard input (0)
        dup(fds); // PTY becomes standard output (1)
        dup(fds); // PTY becomes standard error (2)

        while (1) {
            char input[150];
            rc = read(fds, input, sizeof(input) - 1);

            if (rc > 0) {
                // Replace the terminating \n by a NUL to display it as a string
                input[rc - 1] = '\0';

                printf("Child received : '%s'\n", input);
            } else {
                break;
            }
        }
    }

    return 0;
}

