#include<stdio.h>
int main(void)
{
    //variable declarations
    int ganii_i,ganii_j,ganii_c;

    //code
    printf("\n\n");

    ganii_i = 0;
    do 
    {
        ganii_j = 0;
        do 
        {
            ganii_c = ((ganii_i & 0x8) == 0) ^ ((ganii_j & 0x8) == 0);

            if(ganii_c ==0)
                printf("  ");
            if(ganii_c == 1)
                printf("* ");

            ganii_j++;
        }while(ganii_j < 64);
        
        printf("\n\n");
        ganii_i++;

    }while(ganii_i <64);
    return 0;
}