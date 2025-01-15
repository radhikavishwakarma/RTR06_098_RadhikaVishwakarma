#include <stdio.h>

int main(void)
{
    // variable declarations
    int a;
    int b;
    int x;

    // code
    printf("\n\n");
    printf("----------------------\n");
    printf("Enter a number: ");
    scanf("%d", &a);
    printf("Enter another number: ");
    scanf("%d", &b);

    printf("\n\n");
    
    /* Since, in all the following 5 cases, the operand on the left 'a'
    is getting repeated immediately on the right (e.g: a = a + b or a = a -b)
    */
    // We are using compound assignment operators +=, -=, *=, /= and %=
    /* Since, 'a' will be assigned the value of (a + b) at the expressin
    a += b. We must save the original value of 'a'
    */

    x = a;
    a += b; // a = a + b
    printf("Addition of A = %d and B = %d gives %d.\n", x, b, a);

    // Value of 'a' altered in the above expression is used here
    // Since, 'a' will be assigned the value of (a - b) at the expression
    // (a -= b), we must save the original value of 'a' to another variable (x)
    x = a;
    a -= b; // a = a - b
    printf("Subtraction of A = %d and B = %d gives %d.\n", x, b, a);

    // Value of 'a' altered in the above expression is used here
    // Since, 'a' will be assigned the value of (a * b) at the expression
    // (a *= b), we must save the original value of 'a' to another variable (x)
    x = a;
    a *= b; // a = a * b
    printf("Multiplication of A = %d and B = %d gives %d.\n", x, b, a);

    // Value of 'a' altered in the above expression is used here
    // Since, 'a' will be assigned the value of (a / b) at the expression
    // (a /= b), we must save the original value of 'a' to another variable (x)
    x = a;
    a /= b; // a = a / b
    printf("Division of A = %d and B = %d gives Quotient %d.\n", x, b, a);

    // Value of 'a' altered in the above expression is used here
    // Since, 'a' will be assigned the value of (a % b) at the expression
    // (a %= b), we must save the original value of 'a' to another variable (x)
    x = a;
    a %= b; // a = a / b
    printf("Division of A = %d and B = %d gives Remainder %d.\n", x, b, a);
   
    return(0);
}