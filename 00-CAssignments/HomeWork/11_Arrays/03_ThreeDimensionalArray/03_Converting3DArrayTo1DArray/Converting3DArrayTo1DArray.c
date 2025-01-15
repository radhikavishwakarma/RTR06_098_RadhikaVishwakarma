#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2

int main(void)
{
    // variable declaration
    int iArray_3D[NUM_ROWS][NUM_COLUMNS][DEPTH] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                                                    { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                                                    { { 7, 14 }, { 21, 28 }, { 35, 42 } },
                                                    { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                                                    { { 5, 10 }, { 15, 20 }, { 25, 30 } } };

    int i, j, k;
    int iArray_1D[NUM_ROWS * NUM_COLUMNS * DEPTH];
    int num;

    // code
    // *** Display 3D Array ***
    printf("\n\n");
    printf("Three-Dimensional (3D) Array of Integers : \n\n");
    for ( i = 0; i < NUM_ROWS; i++)
    {
        printf("***** ROW %d *****\n", (i + 1));
        for ( j = 0; j < NUM_COLUMNS; j++)
        {
            for ( k = 0; k < DEPTH; k++)
            {
                printf("iArray_3D[%d][%d][%d] = %d\n", i, j, k,  iArray_3D[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    // *** Converting 3D array to 1D Integer Array
    for ( i = 0; i < NUM_ROWS; i++)
    {
        for ( j = 0; j < NUM_COLUMNS; j++)
        {
            for ( k = 0; k < DEPTH; k++)
            {
                iArray_1D[(i * NUM_COLUMNS * DEPTH) + (j * DEPTH) + k] = iArray_3D[i][j][k];
            }            
        }
    }

    // *** Printing 1D array
    printf("\n\n");
    printf("One-Dimensional (1D) Array of Integers : \n\n");
    for ( i = 0; i < NUM_ROWS * NUM_COLUMNS * DEPTH; i++)
    {
        printf("iArray_1D[%d] = %d\n", i, iArray_1D[i]);
    }
    printf("\n\n");

    return 0;
}
