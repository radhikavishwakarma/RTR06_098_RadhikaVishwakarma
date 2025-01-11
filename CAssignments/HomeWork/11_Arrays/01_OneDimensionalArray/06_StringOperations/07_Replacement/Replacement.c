#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    int MyStrlen(char[]);
    void MyStrcpy(char[], char[]);

    // variable declarations
    char chArray_Original[MAX_STRING_LENGTH], chArray_VowelsReplaced[MAX_STRING_LENGTH]; // A character Array is a string
    int iStringlength, i;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);

    // *** STRING COPY ***
    MyStrcpy(chArray_VowelsReplaced, chArray_Original);

    iStringlength = MyStrlen(chArray_VowelsReplaced);

    for ( i = 0; i < iStringlength; i++)
    {
        switch (chArray_VowelsReplaced[i])
        {
        case 'A':
        case 'a':
        case 'E':
        case 'e':
        case 'I':
        case 'i':
        case 'O':
        case 'o':
        case 'U':
        case 'u':
            chArray_VowelsReplaced[i] = '*';
            break;
        
        default:
            break;
        }
    }
    
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray_Original);

    printf("\n\n");
    printf("String after replacement (i.e : 'chArray_VowelsReplaced[]') is : \n\n");
    printf("%s", chArray_VowelsReplaced);
    
    return 0;
}

void MyStrcpy(char str_destination[], char str_source[])
{
    // function prototype
    int MyStrlen(char[]);

    // variable declarations
    int iStringLength = 0;
    int j;

    // code
    iStringLength = MyStrlen(str_source);
    for ( j = 0; j < iStringLength; j++)
    {
        str_destination[j] = str_source[j];
    }
    str_destination[j] = '\0';
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