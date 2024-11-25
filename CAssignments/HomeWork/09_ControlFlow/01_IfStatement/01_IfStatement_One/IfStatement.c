#include<stdio.h>

int main(void)
{
    // variable declaration
    int a, b, p;

    // code
    a = 9;
    b = 30;
    p = 30;

    printf("\n\n----------------------------------\n");

    if (a < b)
    {
        printf("A is LESS THAN B!!!\n\n");
    }

    if (b != p)
    {
        printf("B is NOT Equal to P!!!\n\n");
    }

    printf("Both comparisions have been done!!!\n");

    return(0);
}