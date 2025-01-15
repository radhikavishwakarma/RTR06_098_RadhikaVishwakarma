#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMN 3

int main(void)
{
    //variable dclaratiosn
    int iArray[NUM_ROWS][NUM_COLUMN];
    int i, j;

    //code
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
            iArray[i][j] = (i + 1) * (j + 1);
    }
    printf("\n\n");
    printf("2D Intger Array Elements Along with Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMN; j++)
        {
            printf("iArray[%d][%d] = %d \t \t At Address : %p \n", i, j, iArray[i][j], &iArray[i][j]);
        }
        printf("\n\n");
    }
    return (0);

}
