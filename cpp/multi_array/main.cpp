#include <stdio.h>

int numbers[][8] = {
    {  0,  1,  2,  3,  4,  5,  6,  7 },
    { 10, 11, 12, 13, 14, 15, 16, 17 },
    { 20, 21, 22, 23, 24, 25, 26, 27 },
    { 30, 31, 32, 33, 34, 35, 36, 37 },
    { 40, 41, 42, 43, 44, 45, 46, 47 },
};    

int main(int argc, const char *argv[])
{
    int row_id = 2;
    int* row = numbers[row_id];
    for (int i=0; i<8; i++) {
        printf("row=%d  col=%d  = %d\n", row_id, i, row[i]); 
    }
    return 0;
}

