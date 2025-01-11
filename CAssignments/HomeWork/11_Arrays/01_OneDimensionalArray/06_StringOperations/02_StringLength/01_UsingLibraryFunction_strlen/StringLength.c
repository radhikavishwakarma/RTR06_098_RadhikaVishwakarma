#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable declarations
    char chArray[MAX_STRING_LENGTH]; // A character Array is a string
    int iStringLength = 0;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String entered by you is : \n\n");
    printf("%s", chArray);

    // *** STRING LENGTH ***
    printf("\n\n");
    iStringLength = strlen(chArray);
    printf("Length of the String is : %d Characters !!!\n\n", iStringLength);
    
    return 0;
}