#include<stdio.h>

int main(void)
{
    //variable declaration
    int ganii_iArray_One[5];
    int ganii_iArray_Two[5][3];
    int ganii_iArray_Three[100][100][5];

    int ganii_num_rows_2D;
    int ganii_num_columns_2D;

    int ganii_num_rows_3D;
    int ganii_num_columns_3D;
    int gnaii_depthe_3D;

    //code
    printf("\n\n");
    printf("Size of 1-D integer array iArray_One = %zu\n",sizeof(ganii_iArray_One));
    printf("Number of elements in 1-D intger arrat iArray_One = %zu\n",(sizeof(ganii_iArray_One)/sizeof(int)));

    printf("\n\n");
    printf("Size of 2-D Intger array iArray_Two = %zu\n",sizeof(ganii_iArray_Two));

    printf("Number rows in 2-D intger array iArray_twp = %zu\n",(sizeof(ganii_iArray_Two)/sizeof(ganii_iArray_Two)));
    ganii_num_rows_2D = (sizeof(ganii_iArray_Two) / sizeof(ganii_iArray_Two));

    printf("NNuumber of elements (columns) in each row in 2-D intger arrat iArray_two = %zu\n",(sizeof(ganii_iArray_Two[0])/ sizeof(ganii_iArray_Two[0][0])));
    ganii_num_columns_2D = (sizeof(ganii_iArray_Two[0]) / sizeof(ganii_iArray_Two[0][0]));

    printf("Number of elements in total in 2-D Arraay iArray_Two = %d\n",(ganii_num_rows_2D * ganii_num_columns_2D));

    printf("\n\n");
    printf("\n\n");

    printf("Size of 3-D intger array iArray_three = %zu\n",sizeof(ganii_iArray_Three));

    printf("Size of 3-D intger array iArray_three = %zu\n",sizeof(ganii_iArray_Three) / sizeof(ganii_iArray_Three[0]));
    ganii_num_rows_3D = (sizeof(ganii_iArray_Three) / (sizeof(ganii_iArray_Three[0])));

    printf("Number of elements (column) in one in 3-D intger arrat iArray_Three = %zu\n",(sizeof(ganii_iArray_Three[0])/sizeof(ganii_iArray_Three[0][0])));
    ganii_num_columns_3D = (sizeof(ganii_iArray_Three[0])/ sizeof(ganii_iArray_Three[0][0]));

    printf("Number of elements (depth) in one column in one row in 3-D intger arrat iArrag_Three= %zu\n",(sizeof(ganii_iArray_Three[0][0]) /sizeof(ganii_iArray_Three[0][0][0])));
    gnaii_depthe_3D = (sizeof(ganii_iArray_Three[0][0]) / sizeof(ganii_iArray_Three[0][0][0]));

    printf("NUmber of elements in total in 3-D Array iArray_Three = %d \n",ganii_num_rows_3D * ganii_num_columns_3D * gnaii_depthe_3D);

    printf("\n\n");
    
    return 0;
}