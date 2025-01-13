#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS_ONE 3
#define NUM_COLUMNS_TWO 8

int main(void)
{
    //variable declaratiosn; //A 2D Array ewhich will have 5 rows and numbers of column can be decided later on..
    int i, j;
    int* iArray[NUM_ROWS];
    //code
    //***One(Allocating memory for an array of 3 intgers per Row) */
    printf("\n\n");
    printf("**First Memory Allocations To 1D Intger Array ***\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray[i] = (int*)malloc(NUM_COLUMNS_TWO * sizeof(int));
        if (iArray[i] == NULL)
        {
            printf("Failed TO Allocate Memory To Row d of 2D Intger Array !! Existing now...Now\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memory Allocation to row %d of 2D Intger Array succeded !!\n\n", i);
        }
    }

    //asigning value to 2D Array..
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_TWO; j++)
        {
            iArray[i][j] = (i + 1) * (j + 1);
        }
    }

    //Displayimng 2D Array...
    printf("\n\n");
    printf("Displaying 2D Array :\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_ONE; j++)
        {
            printf("iArray[%d][%d] = %d \n", i, j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    // Freeeing Memory Assigned to 2D array (Must be Done In Reverse Order)
    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray[i]);
        iArray[i] = NULL;
        printf("Memory Allocated To Row %d of 2D Intger Array Has Beedn Successfully free !!!|n\n", i);
    }

    // *** Two Allocating Memeory for an array of 8 intgers per row

    printf("\n\n");
    printf("****Second Memeory Alloction to 2D Intger Array *****\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        iArray[i] = (int*)malloc(NUM_COLUMNS_TWO * sizeof(int));
        if (iArray[i] == NULL)
        {
            printf("Failed to allocated memeory to row %d of 3d Intger Array !!! Existing now...\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memeory alloctaion To Row %d of 2D Intger Array succeeded !!\n\n",i);
        }
    }

    //Assigning values to 2D Array..
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_TWO; j++)
        {
            iArray[i][j] = (i + 1) * (j + 1);
        }
    }

    // Displaying 2 D Array...
    printf("\n\n");
    printf("Displaying 2 D Array :\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_TWO; j++)
        {
            printf("iArray[%d][%d] = %d\n", i, j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    for (i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray[i]);
        iArray[i] = NULL;
        printf("Memory Allocated to Row %d of 2D Array Has Been successfully freeed !!\n\n", i);
    }

    return (0);

}
