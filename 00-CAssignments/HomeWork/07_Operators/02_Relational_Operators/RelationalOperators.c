#include <stdio.h>

int main(void)
{
    // variable declaration
    int a;
    int b;
    int result;

    // code
    printf("\n\n");
    printf("Enter one integer : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter another integer : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("If Answer = 0, it is 'FALSE'.\n");
    printf("If Answer = 1, it is 'TRUE'.\n");

    result = (a < b);
    printf("(a < b) A = %d is less than B = %d \t Answer = %d\n", a, b, result);

    result = (a > b);
    printf("(a > b) A = %d is greater than B = %d \t Answer = %d\n", a, b, result);

    result = (a <= b);
    printf("(a <= b) A = %d is less than or equal to B = %d \t Answer = %d\n", a, b, result);

    result = (a >= b);
    printf("(a >= b) A = %d is greater than or equal to B = %d \t Answer = %d\n", a, b, result);

    result = (a == b);
    printf("(a == b) A = %d is equal to B = %d \t Answer = %d\n", a, b, result);

    result = (a != b);
    printf("(a != b) A = %d is NOT equal to B = %d \t Answer = %d\n", a, b, result);



    return(0);
}