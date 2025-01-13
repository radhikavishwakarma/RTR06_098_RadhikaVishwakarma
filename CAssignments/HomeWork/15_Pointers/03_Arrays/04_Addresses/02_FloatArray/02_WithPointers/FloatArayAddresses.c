#include<stdio.h>

int main(void)
{
    // Variablde declarations
    float fArray[10];
    float* ptr_fArray = NULL;
    int i;

    //code
    for (i = 0; i < 10; i++)
        fArray[i] = (float)(i + 1) * 1.5f;

    //Name of any array is its base address
    //hence fArray is the base address of array fArray[] or fArray Is The Address of Element fArray[0]
    // Assigning base address of array farray[] to float pointer 'ptr_fArray

    ptr_fArray = fArray;// ptr_fArray = &fArray[0]

    printf("\n\n");
    printf("Elements of the float array :\n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f\n", i, *(ptr_fArray + i));

    printf("\n\n");
    printf("Elements of the float array  : \n\n");
    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t \t Addres = %p \n", i, *(ptr_fArray + i), (ptr_fArray + i));

    printf("\n\n");
    return (0);
}
