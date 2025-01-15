#include <stdio.h>

int main(void)
{
    // variable declaration
    int a;
    int b;
    int result;

    // code
    printf("\n\n");
    printf("---------------------\n");
    printf("Enter a number: ");
    scanf("%d", &a);

    printf("Enter another number: ");
    scanf("%d", &b);

    printf("\n\n");

    /* The following are the 5 Arithmetic Operators +, -, *, / and %
    Also, the resultant of the Arithmetc Operations in all the below
    Five cases have been assigned to the variable 'result' using the 
    assignment operator ( = )
    */

   result = a + b;
   printf("Addition of A = %d and B = %d gives %d.\n", a, b, result);

   result = a - b;
   printf("Subtraction of A = %d and B = %d gives %d.\n", a, b, result);

   result = a * b;
   printf("Multiplication of A = %d and B = %d gives %d.\n", a, b, result);

   result = a / b;
   printf("Division of A = %d and B = %d gives Quotient %d.\n", a, b, result);

   result = a % b;
   printf("Division of A = %d and B = %d gives Remainder %d.\n", a, b, result);


    return(0);
}