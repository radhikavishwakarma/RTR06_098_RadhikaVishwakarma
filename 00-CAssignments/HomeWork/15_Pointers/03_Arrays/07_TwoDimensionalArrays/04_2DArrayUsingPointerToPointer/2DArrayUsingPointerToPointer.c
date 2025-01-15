#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    //variable declarations
    int** ptr_iArray = NULL; //A pointer-to-pointer to intger..but can also hold base address of 2 D Arraya which will cna have any number of rows and any number of column...
    int i, j;
    int num_rows, num_columns;

    //code
    //****Accept Number of rows num_wos from user */
    printf("\n\n");
    printf("Enter Number of Rows : ");
    scanf("%d", &num_rows);

    // ***Accept Number column num_column fro user ***
    printf("\n\n");
    printf("Enter Number of Columns : ");
    scanf("%d", &num_columns);

    // (((Alloctaing memeory to 1D Array Consiting fo Base address of rows ***
    printf("\n\n");
    printf("************Memory Allocations To 2D Intger Array ****\n\n");
    ptr_iArray = (int**)malloc(num_rows * (sizeof(int*)));
    if (ptr_iArray == NULL)
    {
        printf("Failed to Allocate memory to %d Rows of 2D Intger Array !!! Existing now ..\n\n", num_rows);
        exit(0);
    }
    else
    {
        printf("Memory allocation to %d Rows of 2D Intger Array Succeedde !!!|n\n", num_rows);
    }

    // **Allocating memeory to each row which is a 1d constaining cosnisting fo columns which contain the actuial intgers
    for (i = 0; i < num_rows; i++)
    {
        ptr_iArray[i] = (int*)malloc(num_columns * sizeof(int));//allocating memeory (number of columns *sizeof int) to rows  i
        if (ptr_iArray[i] == NULL)// Row 'i' Memroy alloctaed
        {
            printf("Failed TO allocat memeory to columns of Row d of 2D IntgerArray !!! Existing now...\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memory Allocating To Columns of Row %d of 2D Intger Array Succeeded !!!|n\n", i);
        }
    }
    // **FILLING Up Valiues **
    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            ptr_iArray[i][j] = (i * 1) + (j * 1);//can also use: *(*ptr_iArray + i)+j) = (i*1) + (j *1)

        }
    }

    // ***Displaying Values ****
    for (i = 0; i < num_rows; i++)
    {
        printf("Base Address of Ro %d : ptr_iArray[%d] = %p \t At Address : %p \n", i, i, ptr_iArray[i], &ptr_iArray[i]);
    }

    printf("\n\n");

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            printf("ptr_iArray[%d][%d] = %d \t At Addres : %p\n", i, j, ptr_iArray[i][j], &ptr_iArray[i][j]);//can also use *((ptr_iArray+ i) + j) for avlue and *(ptr(iArray + i )+ j) for address..
        }
        printf("\n\n");
    }
    // ***Freeing memeory Allcoated To each Row **
    for (i = (num_rows - 1); i >= 0; i--)
    {
        if (ptr_iArray[i])
        {
            free(ptr_iArray[i]);
            ptr_iArray[i] = NULL;
            printf("Memory Allocated To row %d Has been Successefully freee !!!\n\n", i);

        }
    }

    // *** Freeing Memory allocated to 1d Array consisting of base addresses if rows **
    if (ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;
        printf("Memeory Allcoated to ptr_iArray has been successfully free !!!\n\n");
    }

    return (0);
}

