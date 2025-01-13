#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //varibale delcaration
    int iArray[NUM_ROWS][NUM_COLUMNS];
    int i, j;

    //code
    // **Name of An Array Itself Is its Base address
    // hence iarray is base Address of 2D Intgers array iArray[][]

    //iArray [5][3] => iArrya a 2D Array Having 5 Rows and 3 columns each of these 5 rows is a 1D Intger Array of 3 intgers..
    //iArray[0] =>IS 0the Row..hence is the basee address or row 0
    //iArray [1] => is the 1st row.. hence is the base address of 1
    //iArray[4] => is the 4th row....hence is the base address of row 4

    //(iArray[0]) + 0 => Address of 0th Intger frombase address of 0th row (iArray[0][0])
      //(iArray[0]) + 1 => Address of 0th Intger from base address of 0th row (iArray[0][1])
      //(iArray[0]) + 0 => Address of 0th Intger from base address of 0th row (iArray[0][2])

      //(iArray[1]) + 0 => Address of 0th Intger from base address of 0th row (iArray[1][0])
      //(iArray[1]) + 1 => Address of 0th Intger from base address of 0th row (iArray[1][1])
      //(iArray[1]) + 2 => Address of 0th Intger from base address of 0th row (iArray[1][2])

    //iArray[0],iArray[1]..Are 1D inter Array and Hnece can be treated as 1D Intger Arrays using pointers..
    //iArray is the name and base address of 2D intger array..
    //(*(iArray + 0) +0) = (iArray[0] + 0)= Address of 0the Element from base address of 0th row = (iArray[0] +0) = (iArray[0] [0])
     //(*(iArray + 0) +1) = (iArray[0] + 1)= Address of 0the Element from base address of 1th row = (iArray[0] +1) = (iArray[0] [1])
      //(*(iArray + 0) +2) = (iArray[0] + 2)= Address of 0the Element from base address of 2nd row = (iArray[0] +2) = (iArray[0] [2])

    // (* (iArray + 1) +0) = (iArray[1] + 0) = Address of 0th Elements from basee address 1st ro = (iArray[1] +0) = (iArraay[1][0])
    // (*(iArray + 1) + 1) = (iArray[1] + 1) = Address of 1st Element from base address of 1st ro = (iArray[1] + 1) = (iArray[1][1])'
    // (*(iArray +1) +2) = (iArray[1] + 2) = Address of 2nd Elements from base address of 1st row = (iArray[1] +2) = (iArray[1][2])

    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            *(*(iArray + i) + j) = (i + 1) * (j + 1);//iArray[i] can be treayed as 1D Array using pointers..


        }
    }
    printf("\n\n");
    printf("2D Inter Array Elements Along with Addresse :\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*(*(iArray + %d) +%d) = %d \t \t  At Address (*(iarray + %d) +%d) : %p\n", i, j, *(*(iArray + i) + j), i, j, (*(iArray + i) + j));
        }
        printf("\n\n");
    }

    return (0);

}