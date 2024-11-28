#include<stdio.h>

int main(void)
{
    // variable declaration
    int a, b, p;

    // code
    a = 9;
    b = 30;
    p = 30;

    // *** FIRST if-esle pair ***
    printf("\n\n");
    if (a < b)
    {
        printf("Entering First if-block...\n\n");
        printf("A is Less Than B !!!\n\n");
    }
    else
    {
        printf("Entering First else-block...\n\n");
        printf("A is NOT Less Than B !!!\n\n");
    }
    printf("First if-else Pair Done !!!\n\n");
    
    // *** SECOND if-esle pair ***
    printf("\n\n");
    if (b != p)
    {
        printf("Entering Second if-block...\n\n");
        printf("B is NOT Equal to P !!!\n\n");
    }
    else
    {
        printf("Entering SECOND else-block...\n\n");
        printf("B is NOT Equal to P !!!\n\n");
    }
    printf("Second if-else Pair Done !!!\n\n");

    return 0;
}