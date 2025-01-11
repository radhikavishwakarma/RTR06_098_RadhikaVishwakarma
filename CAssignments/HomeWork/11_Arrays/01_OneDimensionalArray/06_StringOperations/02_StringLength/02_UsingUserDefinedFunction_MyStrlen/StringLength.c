#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype/declaration
    int MyStrlen(char[]);

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
    iStringLength = MyStrlen(chArray);
    printf("Length of the String is : %d Characters !!!\n\n", iStringLength);
    
    return 0;
}

int MyStrlen(char str[])
{
    // variable declarations
    int j;
    int string_length = 0;

    // code
    /*
    *** Determining exact length of the string, by detecting the first
    occurence of Null Terminating character (\0) ***
    */

    for ( j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (str[j] == '\0')
            break;
        else
            string_length++;
    }
    
    return (string_length);
}