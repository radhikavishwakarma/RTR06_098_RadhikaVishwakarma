#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declaations
    int ganii_iArray[NUM_ROWS][NUM_COLUMNS];
    int ganii_i ,ganii_j;

    //code
    /* **Every ro of a 2D Array is an intger array itself comprisng of Num_column Intger Elements
    //There are % rows and 3 column In A 2D Intger Arrat,Each of the 5 rows is 1D Array of 3 Intgers
    // Henceeach of these 5 Rows Themselves being Arraysmwill be the base addresses of their Respective rows*/

    for(ganii_i = 0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j < NUM_COLUMNS; ganii_j++)
            *(ganii_iArray[ganii_i] +ganii_j) = (ganii_i +1) *(ganii_j + 1); // iArray[i] Can be Treated As 1D Array using Pointer...
    }

    printf("\n\n");
    printf("2D Intger Array Elements Along With Addresses : \n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j =0; ganii_j < NUM_COLUMNS; ganii_j++)
        {
            printf("*(iArray[%d] + %d) = %d \t \t At Address (iArray[i] +j) : %p\n",ganii_i,ganii_j,*(ganii_iArray[ganii_i] +ganii_j),(ganii_iArray[ganii_i]+ganii_j));
        }
        printf("\n\n");
    }
    return 0;
}