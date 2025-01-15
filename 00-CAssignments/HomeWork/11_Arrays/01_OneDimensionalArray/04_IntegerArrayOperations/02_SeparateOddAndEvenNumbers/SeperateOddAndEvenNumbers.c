#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable declarations
    int iArray[NUM_ELEMENTS];
    int i, num;

    // code
    printf("\n\n");

    // *** ARRAY ELEMENTS INPUT *** 
    printf("Enter Integer Elements for Array : \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);
        iArray[i] = num;
    }

    // *** SEPARATING OUT EVEN NUMBERS FROM ARRAYY ELEMENTS ***
    printf("\n\n");
    printf("Even Numbers Amongst the Array Elements are: \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray[i] % 2) == 0)
            printf("%d\n", iArray[i]);
    }

    // *** SEPARATING OUT ODD NMBES FROM ARRAY ELEMENTS ***
    printf("\n\n");
    printf("Odd Numbers Amongst the Array Elements are: \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((iArray[i] % 2) != 0)
            printf("%d\n", iArray[i]);
    }

    return 0;
}