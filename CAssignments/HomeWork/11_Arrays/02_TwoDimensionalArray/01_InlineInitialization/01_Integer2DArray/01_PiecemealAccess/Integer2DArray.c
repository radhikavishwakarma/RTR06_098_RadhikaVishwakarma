#include <stdio.h>

int main(void)
{
    // variable declaration
    int iArray[5][3] = {{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15}}; // inline initialization
    int int_size;
    int iArray_size;
    int iArray_num_elements, iArray_num_rows, iArray_num_columns;

    // code
    printf("\n\n");
    int_size = sizeof(int);
    iArray_size = sizeof(iArray);

    printf("Size of Two dimensional (2D) Integer is = %d\n\n", iArray_size);

    iArray_num_rows = iArray_size / sizeof(iArray[0]);
    printf("Number of Rows in Two Dimensional (2D) Integer Array is = %d\n\n", iArray_num_rows);

    iArray_num_columns = sizeof(iArray[0]) / int_size;
    printf("Number of Column in Two Dimensional (2D) Integer Array is = %d\n\n", iArray_num_columns);

    iArray_num_elements = iArray_num_rows * iArray_num_columns;
    printf("Number of Elements in Two dimensional (2D) Integer Array is = %d\n\n", iArray_num_elements);

    printf("\n\n");
    printf("Elements in the 2D Array : \n\n");

    /* Array indices begin from 0, hence 1st row is actually 0th row and 1st column is actually 0th column
    */

    // *** ROW 1 ***
    printf("***** ROW 1 *****\n");
    printf("iArray[0][0] = %d\n", iArray[0][0]);  // COLUMN 1 (Oth Elements) => 1
    printf("iArray[0][2] = %d\n", iArray[0][1]);  // COLUMN 2 (1st Elements) => 2
    printf("iArray[0][3] = %d\n", iArray[0][2]);  // COLUMN 3 (2nd Elements) => 3
    printf("\n\n");

    // *** ROW 2 ***
    printf("***** ROW 2 *****\n");
    printf("iArray[1][0] = %d\n", iArray[1][0]);  // COLUMN 1 (Oth Elements) => 2
    printf("iArray[1][2] = %d\n", iArray[1][1]);  // COLUMN 2 (1st Elements) => 4
    printf("iArray[1][3] = %d\n", iArray[1][2]);  // COLUMN 3 (2nd Elements) => 6
    printf("\n\n");

    // *** ROW 3 ***
    printf("***** ROW 3 *****\n");
    printf("iArray[2][0] = %d\n", iArray[2][0]);  // COLUMN 1 (Oth Elements) => 3
    printf("iArray[2][2] = %d\n", iArray[2][1]);  // COLUMN 2 (1st Elements) => 6
    printf("iArray[2][3] = %d\n", iArray[2][2]);  // COLUMN 3 (2nd Elements) => 9
    printf("\n\n");

    // *** ROW 4 ***
    printf("***** ROW 4 *****\n");
    printf("iArray[3][0] = %d\n", iArray[3][0]);  // COLUMN 1 (Oth Elements) => 4
    printf("iArray[3][2] = %d\n", iArray[3][1]);  // COLUMN 2 (1st Elements) => 8
    printf("iArray[3][3] = %d\n", iArray[3][2]);  // COLUMN 3 (2nd Elements) => 12
    printf("\n\n");

    // *** ROW 5 ***
    printf("***** ROW 5 *****\n");
    printf("iArray[4][0] = %d\n", iArray[3][0]);  // COLUMN 1 (Oth Elements) => 5
    printf("iArray[4][2] = %d\n", iArray[3][1]);  // COLUMN 2 (1st Elements) => 10
    printf("iArray[4][3] = %d\n", iArray[3][2]);  // COLUMN 3 (2nd Elements) => 15
    printf("\n\n");

    return 0;
}