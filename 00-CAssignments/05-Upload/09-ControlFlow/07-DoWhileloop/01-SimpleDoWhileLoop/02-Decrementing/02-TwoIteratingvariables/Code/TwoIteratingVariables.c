#include<stdio.h>
int main(void)
{
    //vaiable declarations
    int ganii_i,ganii_j;

    //code
    printf("\n\n");

    printf("PRinting DIgits 10 to 1 and 100 to 10 :\n\n");

    ganii_i = 10;
    ganii_j = 100;
    do
    {
        printf("\t %d \t %d\n",ganii_i,ganii_j);
        ganii_i--;
        ganii_j = ganii_j - 10;
    }while(ganii_i >= 1,ganii_j >= 10);

    printf("\n\n");

    return 0;
}