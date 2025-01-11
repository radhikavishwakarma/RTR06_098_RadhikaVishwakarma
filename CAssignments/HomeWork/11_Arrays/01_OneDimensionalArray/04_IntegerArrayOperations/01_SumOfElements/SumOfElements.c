#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable declarations
    int iArray[NUM_ELEMENTS];
    int i, num, sum = 0;

    // code
    printf("\n\n");
    printf("Enter Integer Elements for Array : \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);
        iArray[i] = num;
    }

    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        sum = sum + iArray[i];
    }

    printf("\n\n");
    printf("Sum of all elements of Array = %d\n\n", sum);

    return 0;
}