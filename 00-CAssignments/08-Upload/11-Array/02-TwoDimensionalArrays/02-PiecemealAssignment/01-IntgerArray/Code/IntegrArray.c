#include<stdio.h>
int main(void)
{
    //variablde declarations
    int ganii_iArray[3][5]; //3 rows(0,1,2) and 5 column (0,1,2,3,4)
    int ganii_int_size;
    int ganii_intArray_size;
    int ganii_Array_num_elements,ganii_iArray_num_rows,ganii_iArray_num_column;
    int ganii_i,ganii_j;

    // code
    printf("\n\n");

    ganii_int_size = sizeof(int);

    ganii_intArray_size = sizeof(ganii_iArray);
    printf("Size of Two Dimensional (2D) Intger Array Is = %d\n\n",ganii_intArray_size);

    ganii_iArray_num_rows = sizeof(ganii_iArray[0]) / ganii_int_size;
    printf("Number of Columns In Two Dimensional (2D) Intger Array is = %d\n\n",ganii_iArray_num_rows);;

    ganii_iArray_num_column = sizeof(ganii_iArray[0])/ ganii_int_size;
    printf("Number of Columns In Two Dimensional (2D) Intger Array is = %d\n\n",ganii_iArray_num_column);

    ganii_Array_num_elements = ganii_iArray_num_rows * ganii_iArray_num_column;
    printf("Number of Elements In Two Dimensional (2D) Intger Array Is  = %d\n\n",ganii_iArray_num_column);

    printf("\n\n");
    printf("Elements In the 2d Array : \n\n");

    // *** PIECE MEAL AASSIGNMENT*****
    // ****ROW 1
    ganii_iArray[0][0] = 21;
    ganii_iArray[0][1] = 42;
    ganii_iArray[0][2] = 63;
    ganii_iArray[0][3] = 84;
    ganii_iArray[0][4] = 105;

    // *** ROW 2 ****
    ganii_iArray[1][0] = 22;
    ganii_iArray[1][1] = 44;
    ganii_iArray[1][2] = 66;
    ganii_iArray[1][3] = 88;
    ganii_iArray[1][4] = 110;

    // &***** ROW 3 
    ganii_iArray[2][0] = 23 ;
    ganii_iArray[2][1] = 46;
    ganii_iArray[2][2] = 69;
    ganii_iArray[2 ][3] = 92;
    ganii_iArray[2][4] = 115;

    // ** DIsplay
    for(ganii_i =0; ganii_i< ganii_iArray_num_column; ganii_i++)
    {
        printf("******ROW %d ****\n",(ganii_i+1));
        for(ganii_j = 0; ganii_j< ganii_iArray_num_column ;ganii_j++)
        {
            printf("iArray[%d][%d]= %d\n \n",ganii_i,ganii_j,ganii_iArray[ganii_i][ganii_j]);

        }
        printf("\n\n");

    }
    return 0;
}