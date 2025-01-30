#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //varible declarations 
    char ganii_chArray[MAX_STRING_LENGTH];// A Character Array Is A String
    int ganii_i_StringLength = 0;

    // code

    // ** STRING INPUT *****
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(ganii_chArray,MAX_STRING_LENGTH);

    // *** String Output ****
    printf("\n\n");
    printf("String Entred By You Is :\n\n");
    printf("%s\n",ganii_chArray);

    // *** STRING LENGTH ****
    printf("\n\n");
    ganii_i_StringLength = strlen(ganii_chArray);
    printf("Length Of String Is = %d Characters !!!\n\n",ganii_i_StringLength);

    return 0;
}