#include <stdio.h>

#define MAX_STRING_LENGTH 512
#define SPACE ' '
#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'

int main(void)
{
    //  function prototype/declaration
    int MyStrlen(char[]);
    char MyToUpper(char);

    // variable declarations
    char chArray[MAX_STRING_LENGTH], chArray_CapitalizedFirstLetterofEveryWord[MAX_STRING_LENGTH]; // A character Array is a string
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
        if (i == 0) // First letter of any sentence must be a capital letter
        {
            chArray_CapitalizedFirstLetterofEveryWord[j] = MyToUpper(chArray[i]);
        }
        else if (chArray[i] == SPACE)  // First letter of every word in the sentence must be a capital letter. Words are separated by spaces
        {
            chArray_CapitalizedFirstLetterofEveryWord[j] = chArray[i];
            chArray_CapitalizedFirstLetterofEveryWord[j+1] = MyToUpper(chArray[i+1]);
            /* Since, already TWO characters at indices 'i' and 'i+1' have been
            considered in this else-if block... We are extra-incrementing 'i' and 'j' by 1
            */
            j++;
            i++;
        }

        else if((chArray[i] == FULLSTOP) || (chArray[i] == COMMA) || (chArray[i] == EXCLAMATION) || (chArray[i] == QUESTION_MARK) & (chArray[i] != SPACE))
        {
            chArray_CapitalizedFirstLetterofEveryWord[j] = chArray[i];
            chArray_CapitalizedFirstLetterofEveryWord[j+1] = SPACE;
            chArray_CapitalizedFirstLetterofEveryWord[j+2] = MyToUpper(chArray[i+1]);

            /* Since, already TWO characters at indices 'i' and 'i+1' have been considered
            in this else-if block... we are extra incrementing 'i' by 1
            */
            /* Since, already THREE characters at indices 'j' and (j+1) and (j+2) have
            been considered in this else-if block... we are extra incrementing 'j' by 2
            */
            j = j + 2;
            i++;
        }
        else
            chArray_CapitalizedFirstLetterofEveryWord[j] = chArray[i];
        
        j++;
    }

    chArray_CapitalizedFirstLetterofEveryWord[j] = '\0';
    
    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String entered by you is : \n\n");
    printf("%s", chArray);

    printf("\n\n");
    printf("String after capitalizing first letter of every word :\n\n");
    printf("%s\n", chArray_CapitalizedFirstLetterofEveryWord);

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

char MyToUpper(char ch)
{
    // variable declaration
    int num, c;

    // code
    /* ASCII value of 'a' (97) - ASCII value of 'A' (65) = 32
    This subtraction will give the exact difference between the
    upper and lower case counterparts of each letter of the alphabet.
    If this difference is subtracted from the ASCII value of a lower case
    letter, the resultant ASCII value will be that of its upper case
    counterpart, hence helping us to find its upper case letter!!!
    */
    // ASCII Values of 'a' to 'z' => 97 to 122
    // ASCII Values of 'A' to 'Z' => 65 to 90

    num = 'a' - 'A';

    if ((int)ch >= 97 && (int)ch <= 122)
    {
        c = (int)ch - num;
        return((char)c);
    }
    else
    {
        return(ch);
    }
}