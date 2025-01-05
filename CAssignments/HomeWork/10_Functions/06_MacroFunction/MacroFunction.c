#include <stdio.h>
#define MAX_NUMBER(a, b) ((a > b) ? a : b)

int main(int argc, char *argv[], char *envp[])
{
    // variable declarations
    int iNum01;
    int iNum02;
    int iResult;
    
    float fNum_01;
    float fNum_02;
    float fResult;

    // code
    // ***** COMPARING INTEGER VALUES *****
    printf("\n\n");
    printf("Enter an Integer number : ");
    scanf("%d", &iNum01);

    printf("\n\n");
    printf("Enter another Integer number : ");
    scanf("%d", &iNum02);

    iResult = MAX_NUMBER(iNum01, iNum02);

    printf("\n\n");
    printf("Result of Macro Function MAX_NUMBER() : %d\n", iResult);
    printf("\n\n");

    // ***** COMPARING FLOATING-POINT VALUES *****
    printf("\n\n");
    printf("Enter an Floating Point number : ");
    scanf("%f", &fNum_01);

    printf("\n\n");
    printf("Enter another Floating Point number : ");
    scanf("%f", &fNum_02);

    fResult = MAX_NUMBER(fNum_01, fNum_02);

    printf("\n\n");
    printf("Result of Macro Function MAX_NUMBER() : %f\n",fResult);
    printf("\n\n");

    return 0;
}