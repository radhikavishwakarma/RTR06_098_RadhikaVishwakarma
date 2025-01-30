#include<stdio.h>

int main(void)
{
    //fuction prototypes
    void PrintBinaryFormOfNumber(unsigned int);

    //variable declarations
    unsigned int a;
    unsigned int b;
    unsigned int result;

    //code
    printf("\n\n");
    printf("Enter An Intger =");
    scanf("%u",&a);

    printf("\n\n");
    printf("ENter Another Intger =");
    scanf("%u",&b);

    printf("\n\n\n\n");
    result = a | b;
    printf("Bitwise Or-ing of \nA = %d (Decimal) and B = %d (Deciaml) gives result %d (Decimal).\n\n",a,b,result);

    PrintBinaryFormOfNumber(a);
    PrintBinaryFormOfNumber(b);
    PrintBinaryFormOfNumber(result);

     return (0);
}

/* **************8BEGINNERS TO C PROGRAMMING LANGUAGE : PLEASE IGNORE THE CODE OF THE 
FOLLOWING FUCTION SNIPPET 'PrintBinaryFormOfNumber() ' 
*******************YOU MAY COME COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
YOU HAVE COVERD : ARRAYS,LOOPS AND FUCTIONS 
//******************* THE ONLY OBJECTIVE OF WRITING THIS FUCTION WAS TO OBTAIN THE BINARY
REPRESENTATION OF DECIMAL INTGERS SO THAT BIT-WISE AND-ing OR-ing,COMPLEMENT
AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE
*/

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
    //variable declaration
    unsigned int quotient,remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    //code
    for(i =0; i < 8; i++)
        binary_array[i] = 0;
    
    printf("The Binary Form of The Decimal Intger %d Is \t=\t",decimal_number);
    num = decimal_number;
    i = 7;
    while(num != 0)
    {
        quotient = num /2;
        remainder = num %2 ;
        binary_array[i] = remainder;
        i--;
    }

    for( i= 0; i < 8; i++)
        printf("%u",binary_array[i]);
    
    printf("\n\n");
}

