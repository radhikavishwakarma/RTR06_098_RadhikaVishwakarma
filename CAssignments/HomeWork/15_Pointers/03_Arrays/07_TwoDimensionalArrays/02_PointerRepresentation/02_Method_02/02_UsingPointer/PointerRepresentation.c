#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // Variable declarations
    int i, j;
    int** ptr_iArray = NULL;

    // Code
    printf("\n\n");

    // Memory allocation for rows
    ptr_iArray = (int**)malloc(NUM_ROWS * sizeof(int*)); // Array of pointers for rows
    if (ptr_iArray == NULL)
    {
        printf("Memory allocation for the base addresses of %d rows failed! Exiting now...\n\n", NUM_ROWS);
        exit(0);
    }
    else
    {
        printf("Memory allocation for the base addresses of %d rows succeeded!\n\n", NUM_ROWS);
    }

    // Allocating memory for columns in each row
    for (i = 0; i < NUM_ROWS; i++)
    {
        ptr_iArray[i] = (int*)malloc(NUM_COLUMNS * sizeof(int)); // Memory for each row
        if (ptr_iArray[i] == NULL)
        {
            printf("Memory allocation for columns of row %d failed! Exiting now...\n\n", i);
            exit(0);
        }
        else
        {
            printf("Memory allocation for columns of row %d succeeded!\n\n", i);
        }
    }

    // Assigning values
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            ptr_iArray[i][j] = (i + 1) * (j + 1); // Assign values
        }
    }

    // Displaying values
    printf("\n\n");
    printf("2D Integer Array Elements along with their addresses:\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("ptr_iArray[%d][%d] = %d \t At Address: %p\n", i, j, ptr_iArray[i][j], (void*)&ptr_iArray[i][j]);
        }
        printf("\n");
    }

    // Freeing allocated memory
    for (i = NUM_ROWS - 1; i >= 0; i--)
    {
        if (ptr_iArray[i]) // Check if row is allocated
        {
            free(ptr_iArray[i]); // Free row
            ptr_iArray[i] = NULL; // Nullify pointer
            printf("Memory allocated to row %d has been successfully freed!\n", i);
        }
    }

    if (ptr_iArray)
    {
        free(ptr_iArray); // Free base array of pointers
        ptr_iArray = NULL; // Nullify pointer
        printf("Memory allocated to ptr_iArray has been successfully freed!\n\n");
    }

    return (0);
}
