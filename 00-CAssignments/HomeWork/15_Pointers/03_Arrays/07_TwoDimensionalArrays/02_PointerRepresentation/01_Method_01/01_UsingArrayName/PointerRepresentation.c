#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declaations
    int iArray[NUM_ROWS][NUM_COLUMNS];
    int i, j;

    //code
    /**Every ro of a 2D Array is an intger array itself comprising of Num_column Intger Elements
    //There are % rows and 3 column In A 2D Intger Arrat,Each of the 5 rows is 1D Array of 3 Intgers
    //Hence each of these 5 Rows Themselves being Arraysmwill be the base addresses of their Respective rows*/

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0;j < NUM_COLUMNS; j++)
            *(iArray[i] + j) = (i + 1) * (j + 1); // iArray[i] Can be Treated As 1D Array using Pointer...
    }

    printf("\n\n");
    printf("2D Intger Array Elements Along With Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*(iArray[%d] + %d) = %d \t \t At Address (iArray[i] +j) : %p\n", i, j, *(iArray[i] + j), (iArray[i] + j));
        }
        printf("\n\n");
    }
    return (0);
}
