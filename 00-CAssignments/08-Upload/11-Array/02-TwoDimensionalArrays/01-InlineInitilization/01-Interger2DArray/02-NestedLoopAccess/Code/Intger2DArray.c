#include<stdio.h>
int main(void)
{
    //variable declarations
    int ganii_iArray[5][3] = {{1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15}};// IN-LINE INITIALIZATION
    int ganii_int_size;
    int ganii_iArray_size;
    int ganii_iArray_num_elements,ganii_iArray_num_rows,ganii_iArray_num_column;
    int ganii_i,ganii_j;
    // code
    printf("\n\n");

    ganii_int_size = sizeof(int);

    ganii_iArray_size =sizeof(ganii_iArray);
    printf("Size of Two Dimensional (@D) Intger Array Is = %d\n\n",ganii_iArray_size);

    ganii_iArray_num_rows = ganii_iArray_size / sizeof(ganii_iArray[0]);
    printf("Number Rows In Two Dimension (2D) Integer Array Is  = %d\n\n",ganii_iArray_num_rows);

    ganii_iArray_num_column = sizeof(ganii_iArray[0]) /ganii_int_size;
    printf("Number Columns  In Two Dimensional (@d) Intger Array is = %d\n",ganii_iArray_num_column);

    ganii_iArray_num_elements = ganii_iArray_num_rows * ganii_iArray_num_column;
    printf("Number of Elements In Two Dimensional (2D) Intger Array OIs = %d\n",ganii_iArray_num_elements);

    printf("\n\n");
    printf("Elements In The 2 D Array Array :\n\n");

    //** ARRAY INDICES BEGIN FROM 0 ,HENCE,1 ST ROW IS ACTUALLY 0 TH ROW AND 1ST COLUMN IN ACTUALLY 0 TH COLUMN * */]
    for(ganii_i = 0; ganii_i < ganii_iArray_num_column; ganii_i++)
    {
        printf(" ****ROW %D ******\n",(ganii_i+1));
        for(ganii_j = 0; ganii_j < ganii_iArray_num_column; ganii_j++)
        {
            printf("iArray[%d][%d] = %d\n",ganii_i,ganii_j,ganii_iArray[ganii_i][ganii_j]);
        }
        printf("\n\n");
    }
    return 0;

}