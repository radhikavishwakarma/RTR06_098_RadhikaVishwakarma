#include <stdio.h>

int main(void)
{
    // variable declaration
    int a;
    int b;
    int c;
    int result;

    // code
    printf("\n\n");
    printf("Enter First Integer : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Second Integer : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("Enter Third Integer : ");
    scanf("%d", &c);

    printf("\n\n");
    printf("If Answer = 0, It is 'FALSE'.\n");
    printf("If Answer = 1, It is 'TRUE'.\n\n");

    result = (a <= b) && (b != c);
    printf("Logical AND (&&) : Answer is TRUE (1) if and only if BOTH conditions are TRUE.");
    printf("The Answer is FALSE (0), if any ONE or BOTH  conditions are FALSE.\n\n");
    printf("A = %d is less than or equal to B = %d AND B = %d is NOT equal to C = %d    \t Answer = %d\n\n", a, b, b, c, result);

    result = (b >= a) || (a == c);
    printf("Logical OR (||) : Answer is FALSE (0) if and only if BOTH conditions are FALSE.");
    printf("The Answer is TRUE (1), if any ONE or BOTH  conditions are TRUE.\n\n");
    printf("Either B = %d is greater than or equal to A = %d OR A = %d is Equal to C = %d    \t Answer = %d\n\n", b, a, a, c, result);

    result != a;
    printf("A = %d and using logical NOT (!) operator on A gives Result = %d.\n\n", a, result);

    result != b;
    printf("B = %d and using logical NOT (!) operator on B gives Result = %d.\n\n", b, result);

    result != c;
    printf("C = %d and using logical NOT (!) operator on C gives Result = %d.\n\n", c, result);

    result = (!(a <= b) && !(b != c));
    printf("Using Logical NOT (!) on (a <= b) and also on (b != c) and then AND (&&) operation on them gives Result = %d.\n", result);

    return(0);
}