#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // variable declaration
    int iArray_2D[NUM_ROWS][NUM_COLUMNS]; // Total number of elements = NUM_ROWS * NUM_COLUMNS
    int iArray_1D[NUM_ROWS * NUM_COLUMNS];

    int i, j;
    int num;

    // code
    printf("Enter elements of our choice to fill up the integer 2D Array : \n\n");
    for ( i = 0; i < NUM_ROWS; i++)
    {
        printf("For ROW Number %d : \n", (i + 1));
        for ( j = 0; j < NUM_COLUMNS; j++)
        {
            printf("Enter the element number %d : ", (j + 1));
            scanf("%d", &num);
            iArray_2D[i][j] = num;
        }
        printf("\n\n");
    }
    
    // *** Display of 2D Array ***
    printf("\n\n");
    printf("Two-Dimensional (2D) Array of Integers : \n\n");
    for ( i = 0; i < NUM_ROWS; i++)
    {
        printf("***** ROW %d *****\n", (i + 1));
        for ( j = 0; j < NUM_COLUMNS; j++)
        {
            printf("iArray_2D[%d][%d] = %d\n", i, j, iArray_2D[i][j]);
        }
        printf("\n\n");
    }
    
    // *** Converting 2D array to 1D Integer Array
    for ( i = 0; i < NUM_ROWS; i++)
    {
        for ( j = 0; j < NUM_COLUMNS; j++)
        {
            iArray_1D[(i * NUM_COLUMNS) + j] = iArray_2D[i][j];
        }
    }

    // *** Printing 1D array
    printf("\n\n");
    printf("One-Dimensional (1D) Array of Integers : \n\n");
    for ( i = 0; i < NUM_ROWS * NUM_COLUMNS; i++)
    {
        printf("iArray_1D[%d] = %d\n", i, iArray_1D[i]);
    }
    printf("\n\n");

    return 0;
}
