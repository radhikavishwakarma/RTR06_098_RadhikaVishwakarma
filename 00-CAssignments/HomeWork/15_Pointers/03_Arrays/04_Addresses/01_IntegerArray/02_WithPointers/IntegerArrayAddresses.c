#include<stdio.h>

int main(void)
{
    //variable declarations
    int iArray[10];
    int *ptr_iArray = NULL;
    int i;

    //code
    for(i = 0; i <10; i ++)
        iArray[i] = (i +1) *3;

    //Name of Any Array is its base address
    //Hence ,iArray is the base address of array iArray[]  or 'iArray' Is The Address of Element iArray[0]
    //Asssigning base address of array iarray[] To Integer pointer ptr_iArray

    ptr_iArray = iArray ;// ptr_iArray = &iArray[0]

    printf("\n\n");
    printf("ELements of the intger Array : \n\n");
    for(i = 0; i < 10; i++)
        printf("iArray[%d] = %d\n",i,*(ptr_iArray + i));
    
    printf("\n\n");
    printf("Elementsof the Intger Array  : \n\n");
    for(i = 0; i < 10; i++)
        printf("iArray[%d] = %d \t \t Address  = %p \n",i,*(ptr_iArray +i),(ptr_iArray + i));

    printf("\n\n");

    return(0);
    
}
