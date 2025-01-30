#include<stdio.h>

int main(void)
{
    //variable declarations
    int ganii_i,ganii_j;

    //code
    printf("\n\n");

    ganii_i = 1;
    while(ganii_i <= 10)
    {
        printf("i = %d \n",ganii_i);
        printf("\n\n");

        ganii_j =1 ;
        while(ganii_j <= 5)
        {
            printf("\t j = %d \n",ganii_j);
            ganii_j++;
        }
        ganii_i++;
        printf("\n\n");
    }
    return 0;

}