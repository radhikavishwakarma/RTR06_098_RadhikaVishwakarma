#include<stdio.h>

int main(void)
{
    // variable declaration
    int i;

    // code
    printf("\n\n");
    printf("Printing digits 10 to 1 : \n\n");

    i = 10;
    do
    {
        printf("\t%d\n", i);
        i--;
    } while (i >= 1);
    
    printf("\n\n");

    return 0;
}