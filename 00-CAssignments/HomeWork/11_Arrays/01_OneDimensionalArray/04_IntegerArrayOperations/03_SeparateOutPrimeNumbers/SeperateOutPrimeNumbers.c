#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    // variable declarations
    int iArray[NUM_ELEMENTS];
    int i, j, num, count = 0;

    // code
    printf("\n\n");

    // *** ARRAY ELEMENTS INPUT *** 
    printf("Enter Integer Elements for Array : \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &num);
        // If 'num' is negative ( < 0), then convert it to positive (multiply by -1)
        if (num < 0)
            num = -1 * num;
        
        iArray[i] = num;
    }

    // *** PRINTING ENTIRE ARRAY ***
    printf("\n\n");
    printf("Array Elements are : \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        printf("%d\n", iArray[i]);
    }

    // *** SEPARATING OUT PRIME NUMBERS FROM ARRAY ELEMENTS
    printf("\n\n");
    printf("Prime Numbers amongst Array Elements are : \n\n");
    for ( i = 0; i < NUM_ELEMENTS; i++)
    {
        for ( j = 1; j <= iArray[i]; j++)
        {
            if ((iArray[i] % j) == 0)
            {
                count++;
            }
        }
        
        // Number 1 is neither a PRIME number nor a COMPOSITE
        /*
        If a number is PRIME, it is only DIVISIBLE by 1 and itself.
        Hence, if a number is PRIME, the VALUE of 'count' will be exactly 2
        If the value of 'count' is great than 2, the number is DIDVISIBLE by
        numbers other than 1 and itself and hence, it is NOT PRIME
        The value of 'count' will be 1 only if iArray[i] is 1.
        */

        if (count == 2)
        {
            printf("%d\n", iArray[i]);
        }

        count = 0;  // Reset 'count' to 0 for checking the next number...
    }

    return 0;
}