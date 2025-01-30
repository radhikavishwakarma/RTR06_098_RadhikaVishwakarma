#include<stdio.h>
#include<string.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declarations
    char ganii_chArra_Original[MAX_STRING_LENGTH];// A Character Array Is A String

    // code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(ganii_chArra_Original,MAX_STRING_LENGTH);

    // *** String Input ****
    printf("\n\n");
    printf("The Original String Entred By You u  \n\n");
    printf("%s\n",ganii_chArra_Original);

    printf("\n\n");
    printf("The Reversed String Is: \n\n");
    printf("%s\n ",strrev(ganii_chArra_Original));

    return 0;
}
