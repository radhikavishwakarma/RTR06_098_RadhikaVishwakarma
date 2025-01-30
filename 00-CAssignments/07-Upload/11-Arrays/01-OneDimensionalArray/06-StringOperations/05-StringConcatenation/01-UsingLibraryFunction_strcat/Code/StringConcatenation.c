#include<stdio.h>

#define MAX_STRIG_LENGTH 512

int main(void)
{
    //varibale delcarations
    char chArray_One[MAX_STRIG_LENGTH],chArray_Two[MAX_STRIG_LENGTH];
    // ACHaracter Array Is A String

    // code

    // *** String Input **
    printf("\n\n");
    printf("Enter First String : \n\n");
    gets_s(chArray_One,MAX_STRIG_LENGTH);

    printf("\n\n");
    printf("Enter Secod String :\n\n");
    gets_s(chArray_Two,MAX_STRIG_LENGTH);

    // ** String Concat ***
    printf("\n\n");
    printf("**** BEFORE CONCATENATION ******");
    printf("\n\n");
    printf("The Original First String Entred By You  Is :\n\n");
    printf("%s\n",chArray_One);

    printf("\n\n");
    printf("The ORiginal Second String Entred By yopu Is :\n");
    printf("%s\n",chArray_Two);

    strcat(chArray_One,chArray_Two);

    printf("\n\n");
    printf("\n\n");
    printf("****** After Concatenation*******");
    printf("\n\n");
    printf("chArray_One[] Is : \n\n");
    printf("%s\n",chArray_One);

    printf("\n\n");
    printf("chArray_Two[] Is :\n\n");
    printf("%s\n",chArray_Two);
    
    return 0;


}