#include<stdio.h>

int main(void)
{
    // variable declaration
    int a;

    // code
    printf("\n\n----------------------------------\n");

    a = 5;
    if (a) // Non-zero positive value
    {
        printf("if-block 1: 'A' Exists and has value = %d !!!\n\n", a);
    }

    a = -5;
    if (a) // Non-zero negative value
    {
        printf("if-block 1: 'A' Exists and has value = %d !!!\n\n", a);
    }

    a = 0;
    if (a) // Zero value
    {
        printf("if-block 1: 'A' Exists and has value = %d !!!\n\n", a);
    }

    printf("All three If-Statements are done!!!\n");

    return(0);
}