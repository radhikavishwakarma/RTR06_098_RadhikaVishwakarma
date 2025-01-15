#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype/declarations
    int MyStrlen(char[]);

    // variable declarations
    /* A 'STRING' is an Array of Characters... so char[] is a Array and
    hence, char[] is a 'STRING' 
    An Array of char Arrays is and Array of String !!!
    Hence, char[] is one char Array and hence, is one String
    Hence, char[][] is an Array of char Arrays and hence, is an Array of Strings
    
    Here, the String Array can allow a maximum number of 10 Strings (10 rows)
    and each these 10 strings can have only upto 15 Characters maximum (15 columns)
    */

    char strArray[10][15] = {"Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2024-25)", "Of", "ASTROMEDICOMP."}; // Inline initialization
    int iStrLengths[10]; /* 1D Integer Array - stores lengths of those strings at
    corresponding indices at strArray[0], iStrLengths[1] will be the length of the string at string
    at strArray[1]...10 strings, 10 lengths
    */
    int strArray_size;
    int strArray_num_rows;
    int i, j;

    // code
    printf("\n\n");
    strArray_size = sizeof(strArray);
    strArray_num_rows = strArray_size /sizeof(strArray[0]);

    for ( i = 0; i < strArray_num_rows; i++)
    {
        iStrLengths[i] = MyStrlen(strArray[i]);
    }
    
    printf("\n\n");
    printf("The Entire String Array : \n\n");

    for ( i = 0; i < strArray_num_rows; i++)
    {
        printf("%s ", strArray[i]);
    }
    
    printf("\n\n");
    printf("Strings in the 2D Array : \n\n");

    /* Since, char[][] is an array of strings, referencing only by the row number
    (first []) will give the row or the string
    The column number (second []) is the particular character in that string / row
    */

    for ( i = 0; i < strArray_num_rows; i++)
    {
        printf("String Number %d => %s\n\n", (i +1), strArray[i]);
        for ( j = 0; j < iStrLengths[i]; j++)
        {
            printf("Character %d = %c\n", (j +1), strArray[i][j]);
        }
        printf("\n\n");
    }

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