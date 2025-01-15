#include <stdio.h>

int main(void)
{
    // function prototypes
    void PrintBinaryFormOfNumber(unsigned int);

    // variable declaration
    unsigned int a;
    unsigned int b;
    unsigned int result;

    // code
    printf("\n\n");
    printf("Enter an Integer = ");
    scanf("%u", &a);

    printf("\n\n");
    printf("Enter another Integer = ");
    scanf("%u", &b);

    printf("\n\n\n\n");
    result = a & b;
    printf("Bitwise AND of \nA = %d (Decimal) and B = %d (Decimal) gives result %d (Decimal).\n\n", a, b, result);

    PrintBinaryFormOfNumber(a);
    PrintBinaryFormOfNumber(b);
    PrintBinaryFormOfNumber(result);
    return(0);
}

/*------------Beginners to C Programming Language :  Please ignore the code of the following
function snippet 'PrintBinaryFormOfNumber()'---------------------

You may come back to this code and will understand it much better after you have covered:
Arrays, Loops and Functions

The only objective of writing this function was to opbtain the binary representation of decimal integers
so that Bitwise AND, OR, XOR, ONEs COMPLEMENT, LEFT SHIFT, RIGHT SHIFT could be understood with great ease.
*/

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
    // variable declaration
    unsigned int quotient, remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    // code
    for(i = 0; i < 8; i++)
        binary_array[i] = 0;
    
    printf("The binary form of the decimal integer %d is \t\t", decimal_number);
    num = decimal_number;
    i = 7;
    while (num != 0)
    {
        quotient = num / 2;
        remainder = num % 2;
        binary_array[i] = remainder;
        num = quotient;
        i--;
    }
    
    for(i = 0; i < 8; i++)
        printf("%u", binary_array[i]);
    
    printf("\n\n");
}