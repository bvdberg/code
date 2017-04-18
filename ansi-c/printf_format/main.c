/* CpSc 210
   source: https://people.cs.clemson.edu/~rlowe/cs1070/examples/IO/Printf/printf1.c

   printf() example for formated output.

   This example demonstrates numerous options for formatting data
        that is displayed from a program.

*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int a=5;
    int b=-1;
    float x=5.12345;
    double y=5.12345;
    char c='x';
    char str[]="This is a string";

    /* Integer print */
    printf("The sum of %d and %d is %d\n", a, b, a+b);

    /* Fixed field sizes for integer print */
    printf("The sum of %10d and %10d is %10d\n", a, b, a+b);

    /* Floating point output */
    printf("x=%f\n", x);

    /* Fixed field sizes for floating point print */
    printf("x=%5.2f, x=%10.6f\n", x, x);

    /* Double precision output */
    printf("y=%lf, %5.2lf, %10.6lf\n", y, y, y);

    /* Character output */
    printf("c='%c'\n", c);
    printf("Characters 1 and 4 are: '%c', '%c'\n", str[0], str[3]);

    /* String output */
    printf("Str=\"%s\"\n", str);
    printf("At least 6 characters of str=\"%6s\"\n", str);
    printf("6 characters of str=\"%6.6s\"\n", str);
    printf("                   =\"%6.6s\"\n", "short");
    printf("Right aligned in field, str=\"%30s\"\n",str);
    printf("Left aligned in field, str=\"%-30s\"\n", str);

    return 0;
}

