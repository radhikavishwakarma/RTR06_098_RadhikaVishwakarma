#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    void MyStrrev(char[], char[]);

    // variable declarations
    char chArray_Original[MAX_STRING_LENGTH], chArray_Reversed[MAX_STRING_LENGTH]; // A character Array is a string

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);

    // *** STRING COPY ***
    MyStrrev(chArray_Reversed, chArray_Original);
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray_Original);

    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Reversed[]') is : \n\n");
    printf("%s", chArray_Reversed);
    
    return 0;
}

void MyStrrev(char str_destination[], char str_source[])
{
    // function prototype
    int MyStrlen(char[]);

    // variable declarations
    int iStringLength = 0;
    int i, j, len;

    // code
    iStringLength = MyStrlen(str_source);

    // Array indices begin from 0, hence last index will be always (LENGTH - 1) 
    len = iStringLength - 1;

    /* We need to put the character which is at LAST index 'str_source' to the
    FIRST index of 'str_destination'
    and SECOND LAST character of 'str_source' to the SECOND character of 
    'str_destination' and so on...
    */
    for (i = 0, j = len; i < iStringLength, j >= 0; i++, j--)
    {
        str_destination[i] = str_source[j];
    }
    str_destination[i] = '\0';
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