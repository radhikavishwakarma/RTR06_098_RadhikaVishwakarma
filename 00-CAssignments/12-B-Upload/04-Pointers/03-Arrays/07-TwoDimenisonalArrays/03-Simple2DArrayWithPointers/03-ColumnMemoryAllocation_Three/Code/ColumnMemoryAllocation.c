#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
    //variable declarations
    int *iArray[NUM_ROWS];//A 2 D Array which will have 5 rows and number of column can be decided later on...
    int ganii_i,ganii_j;

    //code
    printf("\n\n");
    printf("*****Memory Allocations to 2 D Intger Array ****\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        //ROW 0 WILL HAVE (NUM_COLUMNS - 0) = (5 -0) = 5 COLUMNS..
        //ROW 1 WILL HAVE (NUM_COLUMNS - 1) = (5 -1) = 5 COLUMNS..
        //ROW 2 WILL HAVE (NUM_COLUMNS -2) = (5 -2) = 5 COLUMNS..
        //ROW 3 WILL HAVE (NUM_COLUMNS -3) = (5 -3) = 5 COLUMNS..
        //ROW 4 WILL HAVE (NUM_COLUMNS - 4) = (5 -4) = 5 COLUMNS..

        //Because of this there is no contigous memeory allocation..hence although we may use th data as a 2D Array,it is not reallt 1 2d Array in Memory..

        iArray[ganii_i] = (int *)malloc((NUM_COLUMNS -ganii_i) * sizeof(int));
        if(iArray[ganii_i] == NULL)
        {
            printf("Failed TO Allocated Memory to Row %d if 2D Ineger Array !!! Existing no..\n\n",ganii_i);
            exit(0);
        }
        else 
        {
            printf("Memeory Allocation To Row %d of 2D Integr Array succeeded !!\n\n",ganii_i);
        }
    }
    for(ganii_i =0; ganii_i < 5 ; ganii_i++)
    {
        for(ganii_j=0; ganii_j < (NUM_COLUMNS -ganii_i); ganii_j++)
        {
            iArray[ganii_i][ganii_j] = (ganii_i *1) +(ganii_j *1);
        }
    }
    for(ganii_i =0; ganii_i < 5; ganii_i++)
    {
        for(ganii_j =0; ganii_j <(NUM_COLUMNS -ganii_i); ganii_j++)
        {
            printf("iArray[%d][%d] = %d \t AtAddres : %p\n",ganii_i,ganii_j,iArray[ganii_i][ganii_j],&iArray[ganii_i],ganii_j);
        }
        printf("\n\n");
    }
    for(ganii_i =(NUM_ROWS -1); ganii_i >= 0; ganii_i--)
    {
        free(iArray[ganii_i]);
        iArray[ganii_i] = NULL;
        printf("Memory Allocated to Row %d Has Been successfully Free !!!\n\n",ganii_i);
    }

    return 0;
}