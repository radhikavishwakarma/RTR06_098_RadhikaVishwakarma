#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype
    void MyStrrev(char[],char[]);

    // variable declarations
    char chArray_Original[MAX_STRING_LENGTH], chArray_Reversed[MAX_STRING_LENGTH];
    // A character array is a string

    // code

    // ** STRING Input ***
    printf("\n\n");
    printf("Enter A String :\n\n");
    gets_s(chArray_Original,MAX_STRING_LENGTH);

    // ** STRING REVERSE ****
    MyStrrev(chArray_Reversed,chArray_Original);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entred By ypu is :\n\n");
    printf("%s\n",chArray_Original);

    printf("\n\n");
    printf("The reversed String Is :\n\n");
    printf("%s\n",chArray_Reversed);

    return 0;
}

void MyStrrev(char str_destination[],char str_source[])
{
    // function prottype
    int Mystrlen(char[]);

    // varibalde clarations
    int iStringLength = 0;
    int i ,j ,len;

    // code 
    iStringLength = MyStrlen(str_source);

    //ARRAY INDICES BEGIN FROM 0,HENCE,LAST INDEX WILL ALWAYS BE (LENGTH -1)
    len = iStringLength -1;

    // WE NEED TO PUT THE CHARACTER WHICH IS AT LAST INDEX OF  'str_source' TO THE 
    // FIRST INDEX OF 'str_destiantion'
    // AND SECODN-LAST CHARACTER OF  'str_source' TO THE SECOND CHARACTER OF 'str_destination' and so on...
    for(i = 0, j = len; i <iStringLength, j >= 0; i++,j--)
    {
        str_destination[i] = str_source[j];
    }

    str_destination[i] ='\0';
}

int MyStrlen(char str[])
{
    //varible declarations
    int j;
    int string_length = 0;

    // coed
    // *** DETERMINING EXATC LENGTH OF THE STRING .BY DETECTOING THE FIRST 
    // OCCURANCE OF NULL-TERMINATING CHARACTER (\0) ***
    for(j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if(str[j] == '\0')
            break;
        else
            string_length++;
    }
    return(string_length);
}