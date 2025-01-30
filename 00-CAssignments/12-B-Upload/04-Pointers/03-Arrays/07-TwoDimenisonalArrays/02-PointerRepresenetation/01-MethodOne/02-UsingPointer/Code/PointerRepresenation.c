#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMN 3

int main(void)
{
    //varibe declarations
    int ganii_iArra[NUM_ROWS][NUM_COLUMN];
    int ganii_i,ganii_j;

    int *ptr_iArray_Row= NULL;

    //code
    //** Every Row OF A 2D Array if An Intgers Array Itself Comprising of Num_COlumns Integer Elements */
    //**There Are 5 Roiws and 3 Column In A 2D Intger Arrat,Each of the 5 rows is 1D Array 3 Intgers
    
    // **HEnce Each of these 5 Rows Themselves beigh Array,will be the Base addresses of their Respective Rows */

    for(ganii_i =0; ganii_i <NUM_ROWS; ganii_i++)
    {
        ptr_iArray_Row = ganii_iArra[ganii_i];//iArray[i] Is the base address if ith Row...
        for(ganii_j =0; ganii_j <NUM_COLUMN; ganii_j++)
            *(ptr_iArray_Row +ganii_j) = (ganii_i +1) *(ganii_j +1);
            //*ptr_iArray_Rows(That ius iArray[i] can be treated As 1D Array Using Pointers..)
    }

    printf("\n\n");
    printf("2D Intger Array Elements Along with Addresses : \n\n");
    for(ganii_i =0; ganii_i < NUM_ROWS;ganii_i++)
    {
        ptr_iArray_Row = ganii_iArra[ganii_i];
        for(ganii_j =0; ganii_j < NUM_COLUMN; ganii_j++)
        {
            printf("*(ptr_iArray_Row +%d) = %d \t \t  At Address (ptr_iArray_Row_ + j) : %p \n",ganii_j,*(ptr_iArray_Row +ganii_j),(ptr_iArray_Row +ganii_j));
        }
        printf("\n\n");
    }
    return 0;
}