#include<stdio.h>

#define MAX_STRING_LENGHT 512 

int main(void)
{
    //variable declarations

    // **A String Is and array of charactes...so char[] is a char array and hence char[] is a string..
    // ** An  array of char arrays is an arrany of sstring !!!
    //hnece char[] is one char array and hence,is one string
    //hence char[][] is an array of char arryas and hnce,is an array of string

    // here the string arrat allow a maximum number 5 strings (5 row) and each of these 5 string can ahve only upto 10 characters maximum (10column)
    char strArray[5][10];// 5 (rows (0,1,2,3,4,)) -> 5 string (each string can have a maximum of 10 characterss)
    int char_size;
    int strArray_size;
    int strArray_num_elements,strArray_num_rows,strArray_num_columns;
    int i;

    //code
    printf("\n\n");

    char_size = sizeof(char);

    strArray_size = sizeof(strArray);
    printf("Size of Two Dimensional (2D) character Array (string array is  = %d \n\n)",strArray_size);

    strArray_num_rows = strArray_size /(sizeof(strArray[0]));
    printf("Number of Rows (strings) In Two Dimensional (2D) Characters Array (String Array) is = %d \n\n",strArray_num_rows);

    strArray_num_columns = sizeof(strArray[0]) /char_size;
    printf("Number of Columns In Two Dimensional (3D)Characters Array (String Array) Is = %d \n\n",strArray_num_columns);

    strArray_num_elements = strArray_num_rows * strArray_num_columns;
    printf("Maximum number of Elements (characters) In Two Dimensional (2D) Character Array (Strng Arrya) Is = %d \n\n",strArray_num_elements);

    // ****PIECE-MEAL ASSIGNMENT***
    //****ROW 1 / STRING1** */
    strArray[0][0] = 'M';
    strArray[0][1] = 'Y';
    strArray[0][2] = '\0'; // NULL-teminating characters

    //***Ro 2 /String 2 **
    strArray[1][0] = 'N';
    strArray[1][1] = 'a';
    strArray[1][2] = 'm';
    strArray[1][3] = 'e';
    strArray[1][4] = '\0'; //Null_termianting charactder

    // ***Row 3 /string 3 ***
    strArray[2][0] ='I';
    strArray[2][1] ='s';
    strArray[2][2] ='\0'; //Null -terminating characterss

    // ***Row 4 /String 4***
    strArray[3][0] = 'G';
    strArray[3][1] = 'a';
    strArray[3][2] = 'n';
    strArray[3][3] = 'e';
    strArray[3][4] = 's';
    strArray[3][5] = 'h';
    strArray[3][6] = '\0'; // Null-termianting characters

    // **Row 5 / string 5 ****
    strArray[4][0] = 'G';
    strArray[4][1] = 'a';
    strArray[4][2] = 'i';
    strArray[4][3] = 'k';
    strArray[4][4] = 'w';
    strArray[4][5] = 'a';
    strArray[4][6] = 'd';
    strArray[4][7] = '\0';// Null-terminating characters

    printf("\n\n");
    printf("The String In The 2D Character Array Are : \n\n");

    for(i=0; i < strArray_num_rows; i++)
        printf("%s",strArray[i]);
    
    printf("\n\n");

    return 0;
}

int MyStrlen(char str[])
{
    //variable declarations
    int j ;
    int string_lenght  = 0;

    //code
    //***Detemining exatc lengtn of the string by detetcing the first occurance of null termianting characters (\0) */

    for(j =0; j <MAX_STRING_LENGHT; j++)
    {
        if(str[j] == '\0')
            break;
        else 
            string_lenght++;
    }
    return  string_lenght;
}