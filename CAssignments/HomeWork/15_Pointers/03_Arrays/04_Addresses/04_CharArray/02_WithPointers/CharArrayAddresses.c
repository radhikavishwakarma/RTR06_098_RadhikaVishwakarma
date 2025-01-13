#include<stdio.h>

int main(void)
{
    //variablde declarations
    char cArray[10];
    char* ptr_cArray = NULL;
    int i;

    //code
    for (i = 0; i < 10; i++)
        cArray[i] = (char)(i + 65);

    //Name of Any Array is its base address
    // Hence cArray is ythe base address of array cArray[] or cArray is the address of elememts of cArray[0]
    // Asssigning base address of array carray[] to char pOinter 'ptr_cArray'

    ptr_cArray = cArray; //ptr_cArray = &cArray[0]

    printf("\n\n");
    printf("Elements of the character Array  : \n\n");
    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c\n", i, *(ptr_cArray + i));

    printf("\n\n");
    printf("Elements of the character Array : \n\n");
    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c \t \t Address = %p\n", i, *(ptr_cArray + i), (ptr_cArray + i));

    printf("\n\n");

    return (0);
}
