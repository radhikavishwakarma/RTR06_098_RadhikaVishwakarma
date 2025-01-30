#include<stdio.h>

int main(void)
{
    //fuction prototypes
    void PrintBinaryFormOfNumber(unsigned int);

    //variable declarations
    unsigned int a;
    unsigned int num_bits;
    unsigned int result;

    //code 
    printf("\n\n");
    printf("Enter An Intger = ");
    scanf("%u",&a);

    printf("\n\n");
    printf("By How many bits Do you want to shift A = %d To The Left ? ",a);
    scanf("%u",&num_bits);

    printf("\n\n\nn\n");
    result = a << num_bits;
    printf("Bitwise LEFT-SHIFT BY %d Bits A = %d \nGives The Result = %d (Deciaml) .\n\n",num_bits,a,result);
    PrintBinaryFormOfNumber(a);
    PrintBinaryFormOfNumber(result);

    return  (0);
}
/*  *******8BEGINNERS TO C PROGRAMMING LANGUAGE : PLEASE IGNORE THE CODE OF THE FOLLWOING
SNIPPET 'PrintBinaryFormOfNumber()' ************
************8YOU MAY COME BACK TO THIS CODE AND WILL UNDERSTAND IT MUCH BETTER AFTER
YOU HAVE COVERRED : ARRAYS ,LOOPS AND FUCTIONS 
//************ THE ONLY DEICMAL INTGERS SO THAT BIT-WISE AND-ing,OR-ing,COMPLEMENT
AND BIT-SHIFTING COULD BE UNDERSTOOD WITH GREAT EASE

 */

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
    // variable declarations
    unsigned int quotient,remainder;
    unsigned int num;
    unsigned int binary_array[8];
    int i;

    //code
    for(i = 0; i < 8; i++)
        binary_array[i] = 0;
    
    printf("The Binary Form Of The Deciaml Intgers %d Is\t=\t",decimal_number);
    num = decimal_number;
    i = 7;
    while(num != 0)
    {
        quotient =num /2;
        remainder = num %2 ;
        binary_array[i] = remainder;
        num = quotient;
        i--;
    }

    for(i = 0; i < 8; i++)
        printf("%u",binary_array[i]);
    
    printf("\n\n");


}