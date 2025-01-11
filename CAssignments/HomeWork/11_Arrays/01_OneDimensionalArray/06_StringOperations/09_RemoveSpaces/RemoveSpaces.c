#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    int MyStrlen(char[]);

    // variable declarations
    char chArray[MAX_STRING_LENGTH], chArray_SpacesRemoved[MAX_STRING_LENGTH]; // A character Array is a string
    int iStringLength, i, j;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    iStringLength = MyStrlen(chArray);
    
    j = 0;
    
    for ( i = 0; i < iStringLength; i++)
    {
        if (chArray[i] == ' ')
        {
            continue;
        }
        else
        {
            chArray_SpacesRemoved[j] = chArray[i];
            j++;
        }
    }

    chArray_SpacesRemoved[j] = '\0';
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray);

    printf("\n\n");
    printf("String after removal of SPACES is :\n\n");
    printf("%s\n", chArray_SpacesRemoved);

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