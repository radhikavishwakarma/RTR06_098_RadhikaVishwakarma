#include <stdio.h>

int main(void)
{
    //variable declarations
    int a;
    int b;
    int result;

    //code 
    printf("\n\n");
    printf("Enter One Integer : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Another Integer : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("If Answer = 0, It Is 'FALSE'.\n");
    printf("If Answer = 1, It Is 'TRUE'.\n");

    result = (a < b);
    printf("(a < b) A = %d Is Less Than B = %d                  \t Answer = %d\n", a, b, result);

    result = (a > b);
    printf("(a > b) A = %d Is Greater Than B = %d                \t Answer = %d\n", a, b, result);

    result = (a <= b);
    printf("(a <= b) A = %d Is Less Than Or Equal To B = %d      \t Answer = %d\n", a, b, result);

    result = (a >= b);
    printf("(a >= b) A = %d Is Greater Than Or Equal To B = %d    \t Answer = %d\n", a, b, result);

    result = (a == b);
    printf("(a == b) A = %d Is Equal To B = %d                     \t Answer = %d\n", a, b, result);

    result = (a != b);
    printf("(a != b) A = %d Is NOT Equal To B = %d                  \t Answer = %d\n", a, b, result);

    return(0);
}