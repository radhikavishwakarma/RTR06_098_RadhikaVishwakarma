#include <stdio.h>

int main(void)
{
    //Variable Declarations
    int a = 5;
    int b = 10;

    //code
    printf("\n\n");
    printf("----------------\n");
    printf("A = %d\n", a);
    printf("A = %d\n", a++);
    printf("A = %d\n", a);
    printf("A = %d\n\n", ++a);

    printf("B = %d\n", b);
    printf("B = %d\n", b--);
    printf("B = %d\n", b);
    printf("B = %d\n\n", --b);

    return(0);
}