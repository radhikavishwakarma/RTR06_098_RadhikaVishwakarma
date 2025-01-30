#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // varibal declarations
    char ganii_chArray_Original[MAX_STRING_LENGTH],ganii_chArray_Copy[MAX_STRING_LENGTH];//
    // A Character Array Is A String

    //code

    //  ** STRING INPUT ***
    printf("\n\n");
    printf("ENter A String :\n\n");
    gets_s(ganii_chArray_Original,MAX_STRING_LENGTH);

    //  *** STRING COPY ***
    strcpy(ganii_chArray_Copy,ganii_chArray_Original);

    // ** STRING OUTPUT ***
    printf("\n\n");
    printf("The Original String Entered By You (i.e. :'chArray_Original[]') Is : n\n");
    printf("%s\n",ganii_chArray_Original);

    printf("\n\n");
    printf("The Copied String (i.e. 'chArray_Copy[])' Is : \n\n");

    printf("%s\n",ganii_chArray_Copy);

    return 0;


}