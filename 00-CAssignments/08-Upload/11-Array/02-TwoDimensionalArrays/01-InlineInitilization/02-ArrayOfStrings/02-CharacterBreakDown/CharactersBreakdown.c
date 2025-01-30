#include<stdio.h>

#define MAX_STRINGLENGHT 512

int main(void)
{
    //function prototype
    int MyStrlen(char[]);

    //variable declarations

    /***A String Is Ans array of characters..so char[] is a char array and hence,char [] is a string
     ***An array of char array is an array strings !!!
     ***Hence char[] is one char array and hence is pne string
    ** Hence,char [][] Is ans array of char arrays and hnece is an array fo strings

    // hence the string array can allow maximu of 10 string (10riws) and each fi these 10 string can ahve only upto 15 character maximu(15 column)
    
    Heer,The stringa rray can allow a maximum number of 10 string (10 rows) and each of these 10 string can have only upto 16 characters maximum (16 column)
     */
    char strArray[10][15] = {"Hello!", "Welcom","To","Real","Time","Rendering", "Batch" , "Of","ASTROMEDICOMP" };// In_line Initialization
    int iStrLenghths[10]; //1D Intger Arrat -stores lenght of those strings at conrrespoings indices in strArray[] e.g. istrlenghths [0] will be the length 
    // string at strArray[0] ,iStrlenght[1] will be the length of string at strArray[1] ....10 strings 10 lenght ....
    int strArray_size;
    int strarray_num_rows;
    int i,j;

    //code
    strArray_size = sizeof(strArray);
    strarray_num_rows = strArray_size / sizeof(strArray[0]);

    // storing in kenght of all the strings...
    for(i =0; i < strarray_num_rows; i++)
        iStrLenghths[i] =MyStrlen(strArray[i]);
    
    printf("\n\n");
    printf("The Entire string Array : \n\n");
    for(i =0; i <strarray_num_rows; i++)
        printf("%s",strArray[i]);
    
    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    //Since char[][] is an array of string ,refencing only by the orw nuberS(first[]) will give the row or the string
    // the column number(secod[] is particular is character in that string /row)

    for(i =0; i<strarray_num_rows; i++)
    {
        printf("String Number %d => %s\n\n",(i+1),strArray[i]);
        for(j =0; j < iStrLenghths[i];j++)
        {
            printf("Character %d =  %c \n",(j+1),strArray[i][j]);
        }
        printf("\n\n");
    }
    return 0;
     
}

int MyStrlen(char str[])
{
    //variable declarations
    int j ;
    int string_lenght = 0;

    //code
    //**Determining exact lenght of the string ,by detetcting the first occurance of numm-terminating character (\0) */
    for(j =0; j <MAX_STRINGLENGHT; j++)
    {
        if(str[j] =='\0')
            break;
        else 
            string_lenght++;

    }
    return string_lenght;
}