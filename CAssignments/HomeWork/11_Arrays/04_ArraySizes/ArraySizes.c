#include <stdio.h>

int main(void)
{
    // variable declaration
    int iArray_One[5];
    int iArray_Two[5][3];
    int iArray_Three[100][100][5];

    int num_rows_2D;
    int num_columns_2D;

    int num_rows_3D;
    int num_columns_3D;
    int depth_3D;

    // code
    printf("\n\n");
    printf("Size of 1D integer array iArray_One = %lu\n", sizeof(iArray_One));
    printf("Number of elements in 1D integer array iArray_One = %lu\n", sizeof(iArray_One) / sizeof(int));

    printf("\n\n");
    printf("Size of 2D integer array iArray_Two = %lu\n", sizeof(iArray_Two));

    printf("Number of rows in 2D integer array iArray_Two = %lu\n", sizeof(iArray_Two) / sizeof(iArray_Two[0]));
    num_rows_2D = (sizeof(iArray_Two) / sizeof(iArray_Two[0]));

    printf("Number of elements (columns) in each row in 2D integer array iArray_Two = %lu\n", sizeof(iArray_Two[0]) / sizeof(iArray_Two[0][0]));
    num_columns_2D = (sizeof(iArray_Two[0]) / sizeof(iArray_Two[0][0]));

    printf("Number of elements in total in 2D Array iArray_Two = %d\n", (num_rows_2D * num_rows_2D));

    printf("\n\n");
    
    printf("\n\n");
    printf("Size of 3D integer array iArray_Three = %lu\n", sizeof(iArray_Three));

    printf("Number of rows in 3D integer array iArray_Three = %lu\n", sizeof(iArray_Three) / sizeof(iArray_Three[0]));
    num_rows_3D = (sizeof(iArray_Three) / sizeof(iArray_Three[0]));

    printf("Number of elements (columns) in each row in 3D integer array iArray_Three = %lu\n", sizeof(iArray_Three[0]) / sizeof(iArray_Three[0][0]));
    num_columns_3D = (sizeof(iArray_Three[0]) / sizeof(iArray_Three[0][0]));

    printf("Number of elements (depth) in each row in 3D integer array iArray_Three = %lu\n", sizeof(iArray_Three[0][0]) / sizeof(iArray_Three[0][0][0]));
    depth_3D = (sizeof(iArray_Three[0][0]) / sizeof(iArray_Three[0][0][0]));

    printf("Number of elements in total in 3D Array iArray_Three = %d\n", (num_rows_2D * num_rows_2D * depth_3D));
    
    printf("\n\n");

    return 0;
}