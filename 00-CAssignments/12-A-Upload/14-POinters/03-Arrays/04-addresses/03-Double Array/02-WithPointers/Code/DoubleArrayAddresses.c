#include<stdio.h>

int main(void)
{
    // variable declarations
    double dArray[10];
    double *ptr_dArray = NULL;
    int i ;

    //code
    for(i = 0; i < 10; i++)
        dArray[i] = (float)(i + 1) * 1.333333f;
    
    // NAme of any array is its base address
    // hence dArray is the base address of array dArray[]  or dArray is the Address of elements dArray[0]
    //The Assigning base address of array darray[] to double pointer ptr_darray

    ptr_dArray = dArray;// ptr_dArray = &dArray[0]

    printf("\n\n");

    printf("Elements of the double array : \n\n");
    for(i = 0; i < 10; i++)
        printf("dArray[%d] = %lf\n",i,*(ptr_dArray + i));
    
    printf("\n\n");
    printf("Elements of the double Array  : \n\n");
    for(i = 0; i < 10; i ++)
        printf("dArray[%d] = %lf\t \t  Address = %p \n",i,*(ptr_dArray + i),(ptr_dArray + i));
    
    printf("\n\n");

    return 0;
}