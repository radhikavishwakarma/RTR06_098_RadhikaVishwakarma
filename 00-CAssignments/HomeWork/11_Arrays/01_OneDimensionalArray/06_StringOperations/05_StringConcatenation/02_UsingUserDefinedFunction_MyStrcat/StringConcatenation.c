#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //  function prototype/declaration
    void MyStrcat(char[], char[]);

    // variable declarations
    char chArray_One[MAX_STRING_LENGTH], chArray_Two[MAX_STRING_LENGTH]; // A character Array is a string

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter First String : \n\n");
    gets_s(chArray_One, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("Enter Second String : \n\n");
    gets_s(chArray_Two, MAX_STRING_LENGTH);
    
    // *** STRING CONCAT ***
    printf("\n\n");
    printf("***** BEFORE CONCATENATION *****");
    printf("\n\n");
    printf("The First String entered by you (i.e : 'chArray_One[]') is : \n\n");
    printf("%s", chArray_One);

    printf("\n\n");
    printf("The Second String entered by you (i.e : 'chArray_One[]') is : \n\n");
    printf("%s", chArray_Two);

    // *** STRING CONCAT ***
    MyStrcat(chArray_Two, chArray_One);

    printf("\n\n");
    printf("***** AFTER CONCATENATION *****");
    printf("\n\n");
    printf("'chArray_One[]' is : \n\n");
    printf("%s", chArray_One);

    printf("\n\n");
    printf("'chArray_Two[]' is : \n\n");
    printf("%s", chArray_Two);
    
    return 0;
}

void MyStrcat(char str_destination[], char str_source[])
{
    // function prototype
    int MyStrlen(char[]);

    // variable declarations
    int iStringLength_Source = 0, iStringLength_Destination = 0;
    int i, j;

    // code
    iStringLength_Source = MyStrlen(str_source);
    iStringLength_Destination = MyStrlen(str_destination);

    /* Array indices begin from 0, hence last valid index of Array will be (LENGTH - 1)
    So, CONCATENATION must begin from index number EQUAL to length of the Array 'str_destination'
    We need to put the character which is at first index of 'str_source' to the (LAST INDEX + 1) of
    'str_destination'
    */
    for (i = iStringLength_Destination, j = 0; j < iStringLength_Source; i++, j++)
    {
        str_destination[i] = str_source[j];
    }
    str_destination[i] = '\0';
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