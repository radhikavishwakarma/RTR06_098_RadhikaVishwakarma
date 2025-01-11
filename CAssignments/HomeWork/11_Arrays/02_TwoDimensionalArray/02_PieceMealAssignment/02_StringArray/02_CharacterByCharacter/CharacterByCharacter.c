#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
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

    char strArray[5][12]; // 5 Rows (0, 1, 2, 3, 4) -> 5 Strings (each string can have maximum of 12 characters)
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
    // ***** ROW 1 / STRING 1 *****
    strArray[0][0] = 'M';
    strArray[0][1] = 'y';
    strArray[0][2] = '\0'; // Null termiinating character

    // ***** ROW 2 / STRING 2 *****
    strArray[1][0] = 'N';
    strArray[1][1] = 'a';
    strArray[1][2] = 'm';
    strArray[1][3] = 'e';
    strArray[1][4] = '\0'; // Null termiinating character

    // ***** ROW 3 / STRING 3 *****
    strArray[2][0] = 'I';
    strArray[2][1] = 's';
    strArray[2][2] = '\0'; // Null termiinating character

    // ***** ROW 4 / STRING 4 *****
    strArray[3][0] = 'L';
    strArray[3][1] = 'o';
    strArray[3][2] = 'k';
    strArray[3][3] = 'e';
    strArray[3][4] = 's';
    strArray[3][5] = 'h';
    strArray[3][6] = '\0'; // Null termiinating character

    // ***** ROW 5 / STRING 5 *****
    strArray[4][0] = 'V';
    strArray[4][1] = 'i';
    strArray[4][2] = 's';
    strArray[4][3] = 'h';
    strArray[4][4] = 'w';
    strArray[4][5] = 'a';
    strArray[4][6] = 'k';
    strArray[4][7] = 'a';
    strArray[4][8] = 'r';
    strArray[4][9] = 'm';
    strArray[4][10] = 'a';
    strArray[4][11] = '\0'; // Null termiinating character


    printf("\n\n");
    printf("Strings in the 2D character Array are : \n\n");

    for ( i = 0; i < strArray_num_rows; i++)
    {
        printf("%s ", strArray[i]);
    }
    printf("\n\n");
    
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