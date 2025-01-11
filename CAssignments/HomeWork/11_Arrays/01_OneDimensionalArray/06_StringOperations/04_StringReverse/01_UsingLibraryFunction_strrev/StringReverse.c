#include <stdio.h>
#include <string.h>  // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable declarations
    char chArray_Original[MAX_STRING_LENGTH]; // A character Array is a string

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray_Original);

    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reversed[]') is : \n\n");
    printf("%s", strrev(chArray_Original));
    
    return 0;
}