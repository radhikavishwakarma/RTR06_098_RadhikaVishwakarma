#include<stdio.h>
int main(void)
{
    //variable declarations
    int ganii_i;

    //code 
    printf("\n\n");

    printf("Printing Even Number From 0 to 100 :\n\n");

    for(ganii_i=0 ; ganii_i <= 100; ganii_i++)
    {
        //condition for a number to be even number => divusion of number by
        //leaves bnoo remainder (remainder =0)
        // if remainder is not 0 the number is off number...
        if(ganii_i % 2 != 0)
        {
            continue;
        }
        else 
        {
            printf("\t%d\n",ganii_i);
        }
        

    }
    printf("\n\n");
    return 0;
}