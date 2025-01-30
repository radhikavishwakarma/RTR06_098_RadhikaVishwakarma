#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS_ONE 3
#define NUM_COLUMNS_TWO 8

int main(void)
{
    //variable declaratiosn; //A 2D Array ewhich will have 5 rows and numbers of column can be decided later on..
    int ganii_i,ganii_j;
    int *iArray[NUM_ROWS];
    //code
    //***One(Allocating memory for an array of 3 intgers per Row) */
    printf("\n\n");
    printf("**First Memory Allocations To 1D Intger Array ***\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        iArray[ganii_i] = (int*)malloc(NUM_COLUMNS_TWO * sizeof(int));
        if(iArray[ganii_i] == NULL)
        {
            printf("Failed TO Allocate Memory To Row d of 2D Intger Array !! Existing now...Now\n\n",ganii_i);
            exit(0);
        }
        else 
        {
            printf("Memory Allocation to row %d of 2D Intger Array succeded !!\n\n",ganii_i);
        }
    }

    //asigning value to 2D Array..
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j=0; ganii_j < NUM_COLUMNS_TWO; ganii_j++)
        {
            iArray[ganii_i][ganii_j] =(ganii_i+1)*(ganii_j +1);
        }
    }

    //Displayimng 2D Array...
    printf("\n\n");
    printf("Displaying 2D Array :\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j < NUM_COLUMNS_ONE; ganii_j++)
        {
            printf("iArray[%d][%d] = %d \n",ganii_i,ganii_j,iArray[ganii_i][ganii_j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    // Freeeing Memory Assigned to 2D array (Must be Done In Reverse Order)
    for(ganii_i = (NUM_ROWS -1); ganii_i >= 0; ganii_i--)
    {
        free(iArray[ganii_i]);
        iArray[ganii_i] = NULL;
        printf("Memory Allocated To Row %d of 2D Intger Array Has Beedn Successfully free !!!|n\n",ganii_i);
    }

    // *** Two Allocating Memeory for an array of 8 intgers per row

    printf("\n\n");
    printf("****Second Memeory Alloction to 2D Intger Array *****\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        iArray[ganii_i] = (int*)malloc(NUM_COLUMNS_TWO *sizeof(int));
        if(iArray[ganii_i] == NULL)
        {
            printf("Failed to allocated memeory to row %d of 3d Intger Array !!! Existing now...\n\n",ganii_i);
            exit(0);
        }
        else 
        {
            printf("Memeory alloctaion To Row %d of 2D Intger Array succeeded !!\n\n",ganii_i);
        }
    }

    //Assigning values to 2D Array..
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j = 0; ganii_j < NUM_COLUMNS_TWO; ganii_j++)
        {
            iArray[ganii_i][ganii_j] = (ganii_i+1)*(ganii_j +1);
        }
    }

    // Displaying 2 D Array...
    printf("\n\n");
    printf("Displaying 2 D Array :\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j< NUM_COLUMNS_TWO; ganii_j++)
        {
            printf("iArray[%d][%d] = %d\n",ganii_i,ganii_j,iArray[ganii_i][ganii_j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    for(ganii_i = (NUM_ROWS -1); ganii_i >= 0; ganii_i--)
    {
        free(iArray[ganii_i]);
        iArray[ganii_i] = NULL;
        printf("Memory Allocated to Row %d of 2D Array Has Been successfully freeed !!\n\n",ganii_i);
    }

    return 0;

}