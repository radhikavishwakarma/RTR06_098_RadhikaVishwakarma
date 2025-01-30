#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5 
#define NUM_COLUMNS 3

int main(void)
{
    //variablde declaarations
    int ganii_i,ganii_j ;
    int **ptr_iArray_ = NULL ;
    
    // code 
    // **Every Row of 1 2D Array is An Intger Array Itself comprising of Num_columns Intger Elements
    //There Are 5 Rows and 3 COlumns In A 2D Intger Array.Each of the se 5 Rows IS A 1 D Array of 3 inters.
    //hnce each of these 5 rows Themselves beigh array,will be the base addresses of their respective rows.
    printf("\n\n");

    // **Memory Allocation
    ptr_iArray_ = (int**)malloc(NUM_ROWS *sizeof(int*));// ptr_iArray if the name and base address of 1D Array conatinibg 5 inter pointers to 5 intger array..so it is an array conatining elements of data type (int*)
    if(ptr_iArray_ == NULL)
    {
        printf("Memory Allocation To the 1D Array of abse addresses of %d Rows Faiked !!!! Existing Now... \n\n",NUM_ROWS);
        exit(0);
    }
    else 
    {
        printf("Memory Alloctaion To The 1D Array of abse addresses of %d Rows Has Succeded !!!\n\n",NUM_ROWS);
    }
    // Allocating Memory to each row
    for(ganii_i =0; ganii_i < NUM_ROWS ; ganii_i++)
    {
        ptr_iArray_[ganii_i] = (int*) malloc(NUM_COLUMNS *sizeof(int));//ptr_iArray [i] is the base address of ith ro..
        if(ptr_iArray_ == NULL)
        {
            printf("Memery Allocation To The columns of Row %d Failed !!! existing now...\n\n",ganii_i);
            exit(0);
        }
        else 
        {
            printf("Memeory Allocations to the colimns of row %d Has succeede !!\n\n",ganii_i);
        }
    }
        // Asssigning VAlues
        for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
        {
            for(ganii_j =0; ganii_j < NUM_COLUMNS; ganii_j++)
            {
                *(*(ptr_iArray_ +ganii_i) + ganii_j) = (ganii_i +1) *(ganii_j +1); //ptr_iArray[i][j] = (i+1)*(j +1)
            }
        }

        // **Displaying VAlues
        printf("\n\n");
        printf("2D Inter Arrya Elements Along with addresses : \n\n");
        for(ganii_i =0; ganii_i < NUM_ROWS; ganii_i++)
        {
            for(ganii_j =0; ganii_j < NUM_COLUMNS; ganii_j++)
            {
                printf("ptr_iArray_Row[%d][%d] = %d \t \t At Address &ptr_iArray_Row[%d][%d] : %p\n",ganii_i,ganii_j,ptr_iArray_[ganii_i][ganii_j],ganii_i,ganii_j, &ptr_iArray_[ganii_i][ganii_j]);
            }
            printf("\n\n");
        }
        // ***Fressing Allocated memory
        //freeing memory of each row 
        for(ganii_i= (NUM_ROWS -1); ganii_i>=0; ganii_i--)
        {
            if(*(ptr_iArray_ + ganii_i))// if(ptr_iArray[i])
            {
                free(*(ptr_iArray_ + ganii_i));//free(ptr_iArray[i])
                *(ptr_iArray_ +ganii_i) = NULL;//ptr_iArray[i] = NULL;
                printf("Memrory Allocated To Row %d has Been Successfully freee !!|n\n");
            }
        }

    if (ptr_iArray_)
    {
        free(ptr_iArray_);
        ptr_iArray_ = NULL;
        printf("Memory Allocated To ptr_iArray Has Beend Succesfully Free !!\n\n");
    }

    
    return 0;
}