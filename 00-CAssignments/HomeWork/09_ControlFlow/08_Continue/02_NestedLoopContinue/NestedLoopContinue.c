#include<stdio.h>

int main(void)
{
    // variable declaration
    int i, j;

    // code
    printf("\n\n");
    printf("Outer Loop Prints Odd Numbers between 1 to 10: \n\n");
    printf("Inner Loop Prints Even Numbers between 1 to 10 for every Odd Nnumber printed by Outer Loop. \n\n");

    for (i = 0; i <= 10; i++)
    {
        // condition for a number to be even number => division of number by 2 leaves no remainder (remainder = 0)
        // condition for a number to be odd number => division of number by 2 leaves remainder (remainder = 1)
        // if remainder is not 0, the number is odd number
        if (i % 2 != 0)  // if number (i) is odd
        {
            printf("i = %d\n", i);
            printf("-------------\n");
            for (j = 0; j <= 10; j++)
            {
                if (j % 2 == 0)
                {
                    printf("\tj = %d\n", j);
                }
                else
                {
                    continue;
                }
            }
            printf("\n\n");
        }
        else    // if number (i) is even
        {
            continue;
        }
    }
    printf("\n\n");
    
    return 0;
}