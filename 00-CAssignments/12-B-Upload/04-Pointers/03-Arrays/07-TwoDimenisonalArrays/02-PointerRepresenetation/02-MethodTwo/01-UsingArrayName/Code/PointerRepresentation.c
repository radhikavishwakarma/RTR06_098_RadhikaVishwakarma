#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //varibale delcaration
    int ganii_iArray[NUM_ROWS][NUM_COLUMNS];
    int ganii_i ,ganii_j;

    //code
    // **NAme of An Array Itself Is its Base address
    // hnece iarray is base Address of 2D Intgers array iArray[][]

    //iArray [5][3] => iArrya a 2D Array Having 5 Rows and 3 columns each of these 5 roiws is a 1D Intger Array of 3 intgers..
    //iArray[0] =>IS 0the Row..hence is the basee addre or row 0
    //iArray [1] => is the 1st row.. hence is the base addres of 1
    //iArray[4] => is the 4th row....hence is the base address of row 4

    //(iArray[0]) + 0 => Address of 0th Intger frombase address of 0th row (iArray[0][0])
      //(iArray[0]) + 1 => Address of 0th Intger frombase address of 0th row (iArray[0][1])
      //(iArray[0]) + 0 => Address of 0th Intger frombase address of 0th row (iArray[0][2])
    
      //(iArray[1]) + 0 => Address of 0th Intger frombase address of 0th row (iArray[1][0])
      //(iArray[1]) + 1 => Address of 0th Intger frombase address of 0th row (iArray[1][1])
      //(iArray[1]) + 2 => Address of 0th Intger frombase address of 0th row (iArray[1][2])
    
    //iArray[0],iArray[1]..Are 1D inter Array and Hnece can be treated as 1D Intger Arrays using pointers..
    //iArray is the name and base address of 2D intger arrar..
    //(*(iArray + 0) +0) = (iArray[0] + 0)= Address of 0the Element from base address of 0th row = (iArray[0] +0) = (iArray[0] [0])
     //(*(iArray + 0) +1) = (iArray[0] + 1)= Address of 0the Element from base address of 1th row = (iArray[0] +1) = (iArray[0] [1])
      //(*(iArray + 0) +2) = (iArray[0] + 2)= Address of 0the Element from base address of 2nd row = (iArray[0] +2) = (iArray[0] [2])

    // (* (iArray + 1) +0) = (iArray[1] + 0) = Address of 0th Elements from basee address 1st ro = (iArray[1] +0) = (iArraay[1][0])
    // (*(iArray + 1) + 1) = (iArray[1] + 1) = Address of 1st Element from base address of 1st ro = (iArray[1] + 1) = (iArray[1][1])'
    // (*(iArray +1) +2) = (iArray[1] + 2) = Address of 2nd Elements from base address of 1st row = (iArray[1] +2) = (iArray[1][2])

    for(ganii_i = 0;  ganii_i <NUM_ROWS ; ganii_i++)
    {
        for(ganii_j= 0; ganii_j <NUM_COLUMNS; ganii_j++)
        {
            *(*(ganii_iArray+ ganii_i) +ganii_j) = (ganii_i +1) *(ganii_j + 1);//iArray[i] can be treayed as 1D Array using pointers..

            
        }
    }
    printf("\n\n");
    printf("2D Inter Array Elements Along with Addresse :\n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
    {
        for(ganii_j = 0; ganii_j < NUM_COLUMNS; ganii_j++)
        {
            printf("*(*(iArray + %d) +%d) = %d \t \t  At Address (*(iarray + %d) +%d) : %p\n",ganii_i,ganii_j,*(*(ganii_iArray +ganii_i) +ganii_j),ganii_i ,ganii_j,(*(ganii_iArray +ganii_i) +ganii_j));
        }
        printf("\n\n");
    }
    return 0;
    
 }