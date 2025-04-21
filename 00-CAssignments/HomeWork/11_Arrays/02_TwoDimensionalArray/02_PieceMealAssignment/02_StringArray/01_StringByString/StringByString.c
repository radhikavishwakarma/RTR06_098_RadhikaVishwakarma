#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype/declaration
    void MyStrcpy(char[], char[]);

    // variable declarations
    
    /* A 'STRING' is an Array of characters ... so char[] is a char Array and
    hence char[] is a 'STRING'
    An Array of char Arrays is and Array of Strings !!!
    Hence, char[] is ONE char Array and hence, is one String
    Hence, char[][] is an Array of char Arrays and hence, is an 
    Array of Strings

    Here, the string array can allow a maximum number of 5 strings (5 rows) and
    each of these 5 strings can have only upto 10 characters maximum (10 columns)
    */

    char strArray[5][10]; // 5 Rows (0, 1, 2, 3, 4) -> 5 Strings (each string can have maximum of 10 characters)
    int char_size;
    int strArray_size;
    int strArray_num_elements, strArray_num_rows, strArray_num_columns;
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

    // ***** PIECE-MEAL ASSIGNMENT *****
    MyStrcpy(strArray[0], "My");
    MyStrcpy(strArray[1], "Name");
    MyStrcpy(strArray[2], "Is");
    MyStrcpy(strArray[3], "Radhika");
    MyStrcpy(strArray[4], "Vishwakarma");

    printf("\n\n");
    printf("Strings in the 2D character Array are : \n\n");

    for ( i = 0; i < strArray_num_rows; i++)
    {
        printf("%s ", strArray[i]);
    }
    printf("\n\n");
    
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