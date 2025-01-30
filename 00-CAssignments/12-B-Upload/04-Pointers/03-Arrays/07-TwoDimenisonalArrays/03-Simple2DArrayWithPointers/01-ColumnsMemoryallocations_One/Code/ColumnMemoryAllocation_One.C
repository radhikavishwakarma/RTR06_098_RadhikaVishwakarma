#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMN  3
int main(void)
{
    //variable declarations
    int *iArray[NUM_ROWS]; // a 2D Array whcih wull have 5 rows and number if columns can be decided kater on..
    int ganii_i,ganii_j;

    //code
    printf("\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        iArray[ganii_i] = (int*)malloc(NUM_COLUMN * sizeof(int));
        if(iArray[ganii_i]== NULL)
        {
            printf("Failed To alloctae memeory to row %d of 2D Intger Array !!!Exusting Now...\n\n",ganii_i);
            exit(0);
        }
        else 
        {
            printf("Memory Allocation To Row %d  of 2D Integer Array Succeeded!!\n\n",ganii_i);
        }
    }

    //Assigning values t 2 D Array..
    for(ganii_i =0; ganii_i< NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j< NUM_COLUMN; ganii_j++)
        {
            iArray[ganii_i][ganii_j]= (ganii_i+1)&(ganii_j+1);
        }
    }

    //Displaing 2DArray..
    printf("\n\n");
    printf("Displaying 2DArray:\n\n");
    for(ganii_i=0; ganii_i< NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j< NUM_COLUMN; ganii_j++)
        {
            printf("iArray[%d][%d]= %d\n",ganii_i,ganii_j,iArray[ganii_i][ganii_j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    //Freeing memory assigned to 2D Array (must be done in reverse order)
    for(ganii_i =(NUM_ROWS -1); ganii_i >= 0; ganii_i--)
    {
        free(iArray[ganii_i]);
        iArray[ganii_i] = NULL;
        printf("Memory Allocated to row %d of 2D Intger array has beedn successfully freee!!\n\n",ganii_i);
    }
    return 0;
}
