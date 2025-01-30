#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variblde declarations
    int ganii_iArray_2D[NUM_ROWS][NUM_COLUMNS]; // TOTAL NUMBER OF ELEMENTS = NUM_ROWS * NUM_COLUMNS
    int ganii_iArray_1D[NUM_ROWS * NUM_COLUMNS];

    int ganii_i,ganii_j;
    int ganii_num ;

    //code
    printf("ENter ELements of Ypur Chpice to Fill Up The Intgher 2D Array :\n\n");
    for(ganii_i = 0; ganii_i <NUM_ROWS; ganii_i++)
    {
        printf("For Row Number %d : \n",(ganii_i +1));
        for(ganii_j = 0; ganii_j <NUM_COLUMNS; ganii_j++)
        {
            printf("Enter ELement Number %d : \n",(ganii_j +1));
            scanf("%d",&ganii_num);
            ganii_iArray_2D[ganii_i][ganii_j] = ganii_num;
        }
        printf("\n\n");
    }   

    /* *DISPLAY OF 2D ARRAY*/
    printf("\n]n");
    printf("Two-Dimensional (2D) Array of Intgers :\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {   
        printf("******ROW %d **********\n",(ganii_i+1));
        for(ganii_j =0; ganii_j <NUM_COLUMNS; ganii_j++)
        {
            printf("iArray_2D[%d][%d] = %d\n",ganii_i,ganii_j,ganii_iArray_2D[ganii_i][ganii_j]);
        }
        printf("\n\n");
            
    }

    // ****COVERTING 2D INTGER ARRAY TO 1 D INEGER ARRAY *******88
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j = 0; ganii_j <NUM_COLUMNS; ganii_j++)
        {
            ganii_iArray_1D[(ganii_i * NUM_COLUMNS) +ganii_j] = ganii_iArray_2D[ganii_i][ganii_j];
        }
    }

}