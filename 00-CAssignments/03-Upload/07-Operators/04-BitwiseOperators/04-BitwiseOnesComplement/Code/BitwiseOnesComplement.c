#include<stdio.h>

int main(void)
{
    //fuction prototypes
    void PrintBinaryFormOfNumber(unsigned int);

    //variable declarations
    unsigned int a;
    unsigned int result;

    //code
    printf("\n\n");
    printf("Enter An Intger = ");
    printf("%u",&a);

    printf("\n\n\n\n");
    result = ~a;
    printf("Bitwise COMPLEMENTING Of \nA = %d (Deciaml) gives result %d (Decimal) \n\n",a,result);
    PrintBinaryFormOfNumber(a);
    PrintBinaryFormOfNumber(result);

    return  0;
}
/* ******* BEGINNER TO CPROGRAMMING LANUAGE : PLEASE IGNORE THE CODE OF THE 
FOLLOWING FUCTION SNIPPET 'PrintBinaryFormOfNumber()' 
// *************YOU MAY COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
YOU HAVE COVERED : ARRAYS , LOOPS,AND FUCTIONS
// *********THE ONLY OBJECTIVE OF WRITING THIS FUCTION WAS TO OBTAIN THE BINARY
REPRESENTATION OF DECIAML INTGERS SO THAT BIT-WISE AND-ing,Or-ing,COMPLEMENT
AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE

*/

void PrintBinaryFormOfNumber(unsigned int deciaml_number)
{
    // variable declarations
    unsigned int quotient,remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    //code
    for(i= 0; i < 8; i++)
        binary_array[i] = 0;
    
    printf("The binary Form Of The decimal Intger %d Is\t=\t",deciaml_number);
    num = deciaml_number;
    i = 7;
    while(num != 0)
    {
        quotient = num / 2;
        remainder = num % 2 ;
        binary_array[i] = remainder;
        num = quotient;
        i--;
    }

    for(i =0; i < 8; i++)
        printf("%u",binary_array[i]);
    
    printf("\n\n");
}