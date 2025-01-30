#include<stdio.h>

int main(void)
{
    //fuction prototypes
    void PrintBinaryFormOfNumber(unsigned int);

    //vairable declarations
    unsigned int a;
    unsigned int b ;
    unsigned int result;

    //code
    printf("\n\n");
    printf("Enter An Interger =");
    scanf("%u",&a);

    printf("\n\n");
    printf("Enter Another Intger =");
    scanf("%u",&b);

    printf("\n\n\n\n");
    result = a ^ b;
    printf("Bitwise XOR-ing of \nA = %d (Deciamal) and B = %d (Decimal) gives result %d (Deciaml).\n\n",a,b,result);

    PrintBinaryFormOfNumber(a);
    PrintBinaryFormOfNumber(b);
    PrintBinaryFormOfNumber(result);

    return 0;
}
/*  *************BEGINNERS TO C PROGRAMMING LANGUAGE : PLEASE IGNORE THE CODE OF THE
FOLLOWING FUCTION SNIPPET 'PrintBinaryFormOfNumber()' *************
************** YOU MAY COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
YOU HAVE COVERD : ARRAY ,LOOPS AND FUCTIONS **************
// *********8THE ONLY OBJECTIVE OF WRITING THIS FUCTION WS TO OBTAIN THE BINARY
REPRESENTATION OF DECIMAL INTGERS SO THAT BIT-WISE AND-ing,OR-ing,COMPLEMENT
AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE************
*/

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
    //variable declarations
    unsigned int quotient,remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    //code 
    for(i = 0; i < 8; i++)
        binary_array[i] = 0;
    
    printf("The Binary Form of The Decimal Intger %d Is\t=\t",decimal_number);
    num = decimal_number;
    i = 7;
    while(num != 0)
    {
        quotient = num /2 ;
        remainder = num % 2;
        binary_array[i] = remainder;
        num = quotient;
        i--;
    }

    for(i = 0;i< 8; i++)
        printf("%u",binary_array[i]);

    printf("\n\n");
}