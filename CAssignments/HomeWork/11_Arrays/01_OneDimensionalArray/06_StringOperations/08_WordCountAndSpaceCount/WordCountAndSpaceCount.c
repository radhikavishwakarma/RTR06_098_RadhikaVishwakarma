#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    int MyStrlen(char[]);

    // variable declarations
    char chArray[MAX_STRING_LENGTH]; // A character Array is a string
    int iStringLength, i;
    int word_count = 0, space_count = 0;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    iStringLength = MyStrlen(chArray);

    for ( i = 0; i < iStringLength; i++)
    {
        switch (chArray[i])
        {
        case 32:  // 32 is the ACSII value for space (' ') character
            space_count++;
            break;
        
        default:
            break;
        }
    }

    word_count = space_count + 1;
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray);

    printf("\n\n");
    printf("The Number of SPACES in the input String = %d\n\n", space_count);
    printf("The Number of WORDS in the input String = %d\n\n", space_count);

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