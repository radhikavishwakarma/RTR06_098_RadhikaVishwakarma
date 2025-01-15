#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMN  3
int main(void)
{
    //variable declarations
    int* iArray[NUM_ROWS]; // a 2D Array whcih wull have 5 rows and number if columns can be decided kater on..
    int i, j;

    //code
    printf("\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray[i] = (int*)malloc(NUM_COLUMN * sizeof(int));
        if (iArray[i] == NULL)
        {
            printf("Failed To alloctae memeory to row %d of 2D Intger Array !!!Exiting Now...\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memory Allocation To Row %d  of 2D Integer Array Succeeded!!\n\n", i);
        }
    }

    //Assigning values t 2 D Array..
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
        {
            iArray[i][j] = (i + 1) & (j + 1);
        }
    }

    //Displaying 2DArray..
    printf("\n\n");
    printf("Displaying 2DArray:\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
        {
            printf("iArray[%d][%d]= %d\n", i, j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    //Freeing memory assigned to 2D Array (must be done in reverse order)
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray[i]);
        iArray[i] = NULL;
        printf("Memory Allocated to row %d of 2D Intger array has beedn successfully freee!!\n\n", i);
    }
    return (0);
}
