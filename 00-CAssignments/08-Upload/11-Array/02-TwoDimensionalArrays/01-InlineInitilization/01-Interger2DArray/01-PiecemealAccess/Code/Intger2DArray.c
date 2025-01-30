#include<stdio.h>
int main(void)
{
    // varible declarations 
    int ganii_iArray[5][3] = {{1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15}};//In-Line Initialization
    int ganii_int_size;
    int ganii_i_Array_size;
    int ganii_iArray_num_elements,ganii_iArray_num_rows,ganii_iArray_num_column;

    //code
    printf("\n\n");

    ganii_int_size = sizeof(int);

    ganii_i_Array_size = sizeof(ganii_iArray);
    printf("Size of Two Dimensional (2D) Intger Array Is = %d\n\n",ganii_i_Array_size);

    ganii_iArray_num_rows = ganii_i_Array_size / sizeof(ganii_iArray[0]);
    printf("Number of Rows In Two Dimensional (2D) Integer Array IS = %d\n\n",ganii_iArray_num_rows);

    ganii_iArray_num_column =sizeof(ganii_iArray[0]) /ganii_int_size;
    printf("Number of Columns In Two Dimensinal (2D) Integr Array Is = %d\n\n",ganii_iArray_num_column);

    ganii_iArray_num_elements = ganii_iArray_num_rows * ganii_iArray_num_column;
    printf("Number of Elements In Two Dimensional (2D) Intger array Is = %d\n\n",ganii_iArray_num_elements);

    printf("\n\n");
    printf("Elements In The 2D Array : \n\n");

    // *** ARRAY INDCES BEGIN FROM 0,HENCE 1ST ROW IS ACTUALLY 0TH ROW AND 1ST COLUMN IS ACTUALLY 0TH COLUMN **

    // *** ROW 1 ****
    printf("********ROW 1 *****\n");
    printf("iArray[0][0] = %d\n",ganii_iArray[0][0]); // *** COLUMN1 **(0the Elements)
    printf("iArrat[0][1]= %d\n",ganii_iArray[0][1]);
    printf("iArray[0][2] = %d\n",ganii_iArray[0][2]);// *** Column 3 ** (2nd Elementss=)

    printf("\n\n");

    // *** ROW 2 *****
    printf("******ROW 2 *******\n");
    printf("iArray[1][0] = %d\n",ganii_iArray[1][0]);//*** COLUMN 2 (0th Element)
    //=>2 
    printf("iArray[1][2] = %d\n",ganii_iArray[1][1]);// ** COLUMN 2 **(2st Element)
    //=>4
    printf("iArray[1][2] = %d\n",ganii_iArray[1][2]);// ** COLUMN 3 *** (2nd ELement)
    //=>6

    printf("\n\n");

    //**ROW 3  */
    printf("*********ROw 3 *****\n");
    printf("iArray[2][0] = %d\n",ganii_iArray[2][0]);// *** COLUMN 1 (0th Element)
    // =>3
    printf("iArray[2][1] = %d\n",ganii_iArray[2][1]);//**COlUMN 2  (1st Element)
    // =>6
    printf("iArray[2][2] = %d\n",ganii_iArray[2][2]); // *** Column 2nd ELement
    // =>9

    printf("\n\n");

    // ** ROW 4 ***
    printf("*******ROW 4************\n");
    printf("iArray[3][0] = %d\n",ganii_iArray[3][0]);// *** COLUMN 1 *** (0th Element)
    // =>4 
    printf("iArray[3][1] = %d\n",ganii_iArray[3][1]); //*** COLUMN 2 **** (1st Element);
    // =?8
    printf("iArray[3][2] = %d\n",ganii_iArray[3][2]);// ** COLUMN 3 ***(2nd ELement);
    // =>12

    printf("\n\n");

    // *** ROW 5 ****
    printf("**** ROW 5 *******\n");
    printf("iArray[4][0] = %d\n",ganii_iArray[4][0]); // **** COLUMN *** (0TH ELEMENT)
    //=>5
    printf("iArray[4][1]= %d\n",ganii_iArray[4][1]);// ***COLUMN 2 *** (1ST ELEMENT)
    // =15
    printf("iArray[4][2]= %d\n",ganii_iArray[4][2]);// ***COLUMN 2 *** (1ST ELEMENT)
    printf("\n\n");

    return 0;
}