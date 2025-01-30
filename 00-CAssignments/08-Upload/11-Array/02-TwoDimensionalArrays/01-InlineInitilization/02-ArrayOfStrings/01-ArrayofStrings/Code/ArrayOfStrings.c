#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype
    int MyStrlen(char[]);

    // variable declarations 

    // **  'string' IS AN CHARACTERS ... CH char[] IS A CHar Array And
    // Hence char[] IS A'String '***
    // ** AN ARRAY OF chAr Arrays Is Amd Array Of strings !!!!!***
    // ** Hence char[] Is one char Array And Hence.Is One String ***
    // ** Hence char[][] Is An Array Of char Arrays And Hence, Is An Array Of Strings(())
    

    // Here The String Array can allow a maximum numbe rof 10 string (10rows)
    // and each of these 10 string can have only upto 15 characters maximum(15 column)
    char strArray[10][15] = {"Hello!","Welcome","To","Real","Time","Rendering","Batch","2024-25","of","ASTROMEDICOMP"};
    // IN-LINE INITIALIZATION
    int ganii_char_size;
    int ganii_strArray_size;
    int ganii_str_Array_num_elements,ganii_str_Array_num_rows,ganii_strArray_num_column;
    int ganii_strActual_num_chars = 0;
    int ganii_i ;

    //code
    printf("\n\n");

    ganii_char_size = sizeof(char);

    ganii_strArray_size = sizeof(strArray);
    printf("Size of Two Dimensional 2D  character Array (String Arrya) Is = %d \n\n",ganii_strArray_size);

    ganii_str_Array_num_rows = ganii_strArray_size / sizeof(strArray[0]);
    printf("Number of Rows (Strings) In Two Dimension (@d) character Array (StringArray) Is = %d\n\n",ganii_str_Array_num_rows);

    ganii_strArray_num_column = sizeof(strArray[0]) / ganii_char_size;
    printf("Number of Columns In Tow Dimesional (2D) Character Array(String Array) Is = %d\n\n",ganii_strArray_num_column);

    ganii_str_Array_num_elements = ganii_str_Array_num_rows * ganii_strArray_num_column;
    printf("Maximum Number Of Elements (Character) In Two Dimensionaln (2D) Character Array (String Arayy) Is = %d\n\n",ganii_str_Array_num_elements);

    for(ganii_i =0; ganii_i <ganii_str_Array_num_rows; ganii_i++)
    {
        ganii_strActual_num_chars = ganii_strActual_num_chars + MyStrlen(strArray[ganii_i]);

    }
    printf("Actual Number of Elements(Characters) In Two Dimensional (2D) Character Array (String Aray) Is = %d\n\n",ganii_strActual_num_chars);

    printf("\n\n");
    printf("Strings In The 2D Array :\n\n");

    // **SINCE char[] []is an Array of strings ,referncing only by the row number(first []) will give the row or the string
    //the Column number(secod[] is the particular character in that string /row)

    printf("%s",strArray[0]);
    printf("%s",strArray[1]);
    printf("%s",strArray[2]);
    printf("%s",strArray[3]);
    printf("%s",strArray[4]);
    printf("%s",strArray[5]);
    printf("%s",strArray[6]);
    printf("%s",strArray[7]);
    printf("%s",strArray[8]);
    printf("%s",strArray[9]);

    return 0;
}

int MyStrlen(char str[])
{
    //varibal declarations
    int ganii_j;
    int ganii_string_length = 0;

    // code
    // ** DETERMINING EXACT LENGTH OF THE STRING ,BY DETETCTING THE FIRST
    // Occurence of NULL-TEMINATINTING CHARACTER (\0) 
    for(ganii_j = 0; ganii_j < MAX_STRING_LENGTH; ganii_j++)
    {
        if(str[ganii_j] == '\0')
            break;
        else
            ganii_string_length++;
        
    }
    return ganii_string_length;
}
