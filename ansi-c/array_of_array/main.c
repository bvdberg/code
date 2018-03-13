#include <stdio.h>

// Correct, 2 arrays of 6 bytes
int data[2][6];

int main(int argc, const char *argv[])
{
    printf("data[2][6] is 2 arrays of 6 bytes. data[0] and data[1] differ 6 bytes\n");
    int* p = &data[0][0];
    for (unsigned i=0; i<12; i++) {
        *p++ = i;
    }

    for (unsigned i=0; i<1; i++) {
        for (unsigned j=0; j<6; j++) {
            printf("[%d][%d] -> %2d\n", i, j, (int)(&data[i][j] - &data[0][0]));
        }
        printf("\n");
    }
    for (unsigned i=0; i<1; i++) {
        for (unsigned j=0; j<6; j++) {
            printf("[%d][%d] -> %2d\n", j, i, (int)(&data[j][i] - &data[0][0]));
        }
        printf("\n");
    }
    for (unsigned i=0; i<2; i++) {
        printf("[%d] -> %2d\n", i, (int)((int*)&data[i] - (int*)&data[0][0]));
    }

    return 0;
}

