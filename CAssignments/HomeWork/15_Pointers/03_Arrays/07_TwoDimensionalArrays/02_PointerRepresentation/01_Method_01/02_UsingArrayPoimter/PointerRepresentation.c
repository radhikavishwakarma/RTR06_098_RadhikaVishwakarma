#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMN 3

int main(void)
{
    //variable declarations
    int iArray[NUM_ROWS][NUM_COLUMN];
    int i, j;

    int* ptr_iArray_Row = NULL;

    //code
    //** Every Row OF A 2D Array if An Intgers Array Itself Comprising of Num_COlumns Integer Elements */
    //**There Are 5 Roiws and 3 Column In A 2D Intger Arrat,Each of the 5 rows is 1D Array 3 Intgers

    // **HEnce Each of these 5 Rows Themselves beigh Array,will be the Base addresses of their Respective Rows */

    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray_Row = iArray[i];//iArray[i] Is the base address if ith Row...
        for (j = 0; j < NUM_COLUMN; j++)
            *(ptr_iArray_Row + j) = (i + 1) * (j + 1);
        //*ptr_iArray_Rows(That ius iArray[i] can be treated As 1D Array Using Pointers..)
    }

    printf("\n\n");
    printf("2D Intger Array Elements Along with Addresses : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray_Row = iArray[i];
        for (j = 0; j < NUM_COLUMN; j++)
        {
            printf("*(ptr_iArray_Row +%d) = %d \t \t  At Address (ptr_iArray_Row_ + j) : %p \n", j, *(ptr_iArray_Row + j), (ptr_iArray_Row + j));
        }
        printf("\n\n");
    }
    return (0);
}
