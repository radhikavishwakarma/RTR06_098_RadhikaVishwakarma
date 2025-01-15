#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
    //variable declarations
    int* iArray[NUM_ROWS];// 2D Array which will have 5 rows and number of column can be decided later on...
    int i, j;

    //code
    printf("\n\n");
    printf("*****Memory Allocations to 2 D Intger Array ****\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        //ROW 0 WILL HAVE (NUM_COLUMNS - 0) = (5 -0) = 5 COLUMNS..
        //ROW 1 WILL HAVE (NUM_COLUMNS - 1) = (5 -1) = 5 COLUMNS..
        //ROW 2 WILL HAVE (NUM_COLUMNS -2) = (5 -2) = 5 COLUMNS..
        //ROW 3 WILL HAVE (NUM_COLUMNS -3) = (5 -3) = 5 COLUMNS..
        //ROW 4 WILL HAVE (NUM_COLUMNS - 4) = (5 -4) = 5 COLUMNS..

        //Because of this there is no contigeous memeory allocation..hence although we may use th data as a 2D Array,it is not reallt 1 2d Array in Memory..

        iArray[i] = (int*)malloc((NUM_COLUMNS - i) * sizeof(int));
        if (iArray[i] == NULL)
        {
            printf("Failed TO Allocated Memory to Row %d if 2D Ineger Array !!! Existing no..\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memeory Allocation To Row %d of 2D Integr Array succeeded !!\n\n", i);
        }
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < (NUM_COLUMNS - i); j++)
        {
            iArray[i][j] = (i * 1) + (j * 1);
        }
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < (NUM_COLUMNS - i); j++)
        {
            printf("iArray[%d][%d] = %d \t AtAddres : %p\n", i, j, iArray[i][j], &iArray[i], j);
        }
        printf("\n\n");
    }
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray[i]);
        iArray[i] = NULL;
        printf("Memory Allocated to Row %d Has Been successfully Free !!!\n\n", i);
    }

    return (0);
}