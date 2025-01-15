#include<stdio.h>

int main(void)
{
    // variable declaration
    int i;

    // code
    printf("\n\n");
    printf("Printing digits 1 to 10 : \n\n");

    i = 1;
    while (i <= 10)
    {
        printf("\t%d\n", i);
        i++;
    }
    
    printf("\n\n");

    return 0;
}