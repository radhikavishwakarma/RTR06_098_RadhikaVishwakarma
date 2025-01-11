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
    int char_size;
    int strArray_size;
    int strArray_num_elements, strArray_num_rows, strArray_num_columns;
    int strActual_num_chars = 0;
    int i;

    // code
    printf("\n\n");
    char_size = sizeof(char);
    strArray_size = sizeof(strArray);
    printf("Size of Two dimensional (2D) Character Array (String Array) is = %d\n\n", strArray_size);

    strArray_num_rows = strArray_size /sizeof(strArray[0]);
    printf("Number of Rows (Strings) in Two Dimensional (2D) Character Array (String Array) is = %d\n\n", strArray_num_rows);

    strArray_num_rows = sizeof(strArray[0]) / char_size;
    printf("Number of Columns in Two Dimensional (2D) Character Array (String Array) is = %d\n\n", strArray_num_columns);

    strArray_num_elements = strArray_num_rows * strArray_num_columns;
    printf("Maximum Number of Elements (Characters) in TWO Dimensional (2D) Character Array (String Array) is = %d\n\n", strArray_num_elements);

    for ( i = 0; i < strArray_num_rows; i++)
    {
        strActual_num_chars = strActual_num_chars + MyStrlen(strArray[i]);
    }
    printf("Actual number of Elements (Characters) in TWO Dimensional (2D) is = %d \n\n", strActual_num_chars);
    
    printf("\n\n");
    printf("Strings in the 2D Array : \n\n");

    /* Since, char[][] is an array of strings, referencing only by the row number
    (first []) will give the row or the string

    The Column number (second []) is the particular character in that string / row
    */

    printf("%s ", strArray[0]);
    printf("%s ", strArray[1]); 
    printf("%s ", strArray[2]);
    printf("%s ", strArray[3]);
    printf("%s ", strArray[4]);
    printf("%s ", strArray[5]);
    printf("%s ", strArray[6]);
    printf("%s ", strArray[7]);
    printf("%s ", strArray[8]);
    printf("%s\n\n", strArray[9]);

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