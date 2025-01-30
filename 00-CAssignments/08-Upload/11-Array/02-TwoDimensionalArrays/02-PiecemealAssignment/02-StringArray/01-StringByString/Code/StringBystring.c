#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void MyStrcpy(char[],char[]);

    //variablde declarations

    /* A 'STRING' IS AN ARRAY OF CHARACTERS.. so char[] IS A char ARRAY AND HENCE char[] IS A 'STRING'
    *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! 
    // **HENCE,char[] IIS ONE char ARRAY AND HENCE,IS ONE STRING ***
    // **HENCE,char[][]IS AN ARRAY OF char ARRAYS AND HENCE,IS AN ARRAY OF STRING***

    // Here ,the string array can allow maximum number of 5 strings(5rows) and each of the 5 string can have only upto
    10 characters  maximum(10 column)
    
    */
   char ganii_strArray[5][10]; // 5 ROWS(0,1,2,3,4)->5 STRING(EACH STRING CAN HAVE MAXIMUM OF 10 CHARACTERS)
   int ganii_char_size;
   int ganii_strArray_size;
   int ganii_strArray_num_elements,ganii_strArray_num_rows,ganii_strArray_num_columns;
   int ganii_i;

   //code
   printf("\n\n");
   ganii_char_size = sizeof(char);

   ganii_strArray_size = sizeof(ganii_strArray);
   printf("Size of Two Dimensional (2D) cCharacter Array (String Array) Is = %d\n\n",ganii_strArray_size);

   ganii_strArray_num_rows = ganii_strArray_size / sizeof(ganii_strArray[0]);
   printf("Number of Rows(String) In Two Dimensional (2 D) character Array (String Array) Is = %d \n\n",ganii_strArray_num_rows);

    ganii_strArray_num_columns = sizeof(ganii_strArray[0]) / ganii_char_size ;
    printf("Number of Columns In Two Dimensional (2D) Characters Array (String Array) Is = %d\n\n",ganii_strArray_num_columns);

    ganii_strArray_num_elements = ganii_strArray_num_rows * ganii_strArray_num_columns;
    printf("Maximjum Number of Elements (Characters) In Two Dimensional(2D) Character Array (string Array) Is  = %d\n\n",ganii_strArray_num_elements);

    // *** PIECE-MEAL ASSIGNMENT***
    MyStrcpy(ganii_strArray[0],"My");
    MyStrcpy(ganii_strArray[1],"Name");
    MyStrcpy(ganii_strArray[2],"IS");
    MyStrcpy(ganii_strArray[3],"Ganesh");
    MyStrcpy(ganii_strArray[4],"Gaikwad");

    printf("\n\n");
    printf("Th Strings In The 2D Character Array Are : \n\n");

    for(ganii_i = 0; ganii_i < ganii_strArray_num_rows; ganii_i++)
        printf("%s",ganii_strArray[ganii_i]);
    
    printf("\n\n");

    return 0;

}

void MyStrcpy(char str_destination[],char str_source[])
{
    //function prototype
    int MyStrlen(char[]);

    //variablde declarations
    int iStringLength = 0;
    int j;

    //code
    iStringLength = MyStrlen(str_source);
    for(j = 0; j< iStringLength;j++)
        str_destination[j] = str_source[j];
    str_destination[j] = '\0';
}

int MyStrlen(char str[])
{
    //variable declarations
    int j;
    int stringLength = 0;

    //code
    /* **DETERMINING EXACT LENGTH OF THE STRING,BY DETETCTING THE FIRST 
    OCCURANCE OF NULL-TERMINATING CHARACTER (\0)*/
    for(j =0; j <MAX_STRING_LENGTH; j++)
    {
        if(str[j] == '\0')
            break;
        else
            stringLength++;
    }
    return  stringLength;
    
}
