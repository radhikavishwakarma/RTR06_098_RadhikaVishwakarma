#include<stdio.h>
int main(void)
{
    //varianble declarations 
    int ganii_i,ganii_j;

    //code
    printf("\n\n");

    printf("Outer Loop Prints Odd Numbers Between 1 and 100 \n\n");
    printf("Inner Loop Prints Even Numbers Between 1 and 10 For Every Odd Number Printed By Outer Loop...\n\n");
    //condition for a number to be evenn number => division of number by 2 leaves
    //no remainder (remainder =0)
    // condition for a number to be a odd number => division of number by 2 leaves
    //remainder (remainder =1 (usually))

    for(ganii_i =1; ganii_i <= 10; ganii_i++)
    {
        if(ganii_i %2 != 0)//If Number (i) Is Odd
        {
            printf("i= %d\n",ganii_i);
            printf("------------\n");
            for(ganii_j = 1; ganii_j<= 10; ganii_j++)
            {
                if(ganii_j %2 ==0)// If number (j) is Even..
                {
                    printf("\tj %d\n",ganii_j);
                }
                else//If number (j) is Odd..
                {
                    continue;
                }
            }
            printf("\n\n");
        }
        else // If Number (i) Is Even...
        {
            continue;
        }
    }
    printf("\n\n");

    return 0;
}
