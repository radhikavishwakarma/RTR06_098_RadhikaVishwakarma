#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function type
    void MyStrcpy(char[],char[]);

    //vaiblde declarattions 
    char ganii_chArray_Original[MAX_STRING_LENGTH],ganii_chArray_Copy[MAX_STRING_LENGTH];
    // A character Array Is String

    //code
    printf("\n\n");
    printf("Enter String : \n\n");
    gets_s(ganii_chArray_Original,MAX_STRING_LENGTH);

    // ** String Copy ***
    MyStrcpy(ganii_chArray_Copy,ganii_chArray_Original);

    // ** STRING OUTPUT ****
    printf("\n\n");
    printf("The Original String-Entered By You Is :\n\n");
    printf("%s\n",ganii_chArray_Original);

    printf("\n\n");
    printf("The Copied String(ie. : chArray_Copy[]) Is :\n\n");
    printf("%s\n",ganii_chArray_Copy);

    return 0;

}

void MyStrcpy(char str_destination[],char str_source[])
{
    //function prototype
    int MyStrlen(char[]);

    //variable delcarations
    int iStringLength = 0;
    int ganii_j ;

    //code 
    iStringLength = MyStrlen(str_source);
    for(ganii_j = 0; ganii_j < iStringLength;ganii_j++)
        str_destination[ganii_j] = str_source[ganii_j];
    
    str_destination[ganii_j] = '\0';
    
}

int MyStrlen(char str[])
{
    //varible declarations
    int ganii_j;
    int ganii_string_length = 0;

    // code
    // ***DETERMNING EXACT LENGTH OF THE STRING ,BY DETECTING THE FIRST OCCURANCE OF NULL-TERMINATING CHARACTER (\0)
    for(ganii_j = 0; ganii_j < MAX_STRING_LENGTH; ganii_j++)
    {
        if(str[ganii_j] =='\0')
            break;
        else    
            ganii_string_length;

    }
    return  ganii_string_length;

}