#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    int MyStrlen(char[]);

    // variable declarations
    char chArray[MAX_STRING_LENGTH]; // A character Array is a string
    int iStringLength;
    int count_A = 0, count_E = 0, count_I = 0, count_O = 0, count_U = 0; // Initial Counts = 0
    int i;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter a String : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The String entered by you is : \n\n");
    printf("%s", chArray);

    iStringLength = MyStrlen(chArray);
    
    for (i = 0; i < iStringLength; i++)
    {
        switch (chArray[i])
        {
        case 'A':
        case 'a':
            count_A++;
            break;
        case 'E':
        case 'e':
            count_E++;
            break;
        case 'I':
        case 'i':
            count_I++;
            break;
        case 'O':
        case 'o':
            count_O++;
            break;
        case 'U':
        case 'u':
            count_U++;
            break;
        default:
            break;
        }
    }

    printf("\n\n");
    printf("In the string entered by you, the vowels and the number of their occurences are as follows:\n");
    printf("'A' has occured = %d times !!!\n", count_A);
    printf("'E' has occured = %d times !!!\n", count_E);
    printf("'I' has occured = %d times !!!\n", count_I);
    printf("'O' has occured = %d times !!!\n", count_O);
    printf("'U' has occured = %d times !!!\n", count_U);

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