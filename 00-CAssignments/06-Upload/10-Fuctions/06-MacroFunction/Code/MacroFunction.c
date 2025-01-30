#include<stdio.h>

#define MAX_NUMBER(a,b) ((a>b) ? a: b)

int main(int argc, char *argv[],char *envp[])
{
    //varible declarations
    int iNum_01;
    int iNum_02;
    int iResult;

    float fNum_01;
    float fNum_02;
    float fResult ;

    //code 
    // ******COMPARING INTGER VALUES **********8
    printf("\n\n");
    printf("Enter An Intger Number : \n\n");
    scanf("%d",&iNum_01);

    printf("\n\n");
    printf("Enter Another Intger Number : \n\n");
    scanf("%d",&iNum_02);

    iResult = MAX_NUMBER(iNum_01,iNum_02);
    printf("\n\n");
    printf("Result of Macro Function MAX_NUMBER() = %d\n",iResult);

    printf("\n\n");

    // ****COMPARING FLOATING-POINT VALUE ***********
    printf("\n\n");
    printf("Enter A Floating Point Number : \n\n");
    scanf("%f",&fNum_01);

    printf("\n\n");
    printf("Enter Another Floating Point Number : \n\n");
    scanf("%f",&fNum_02);

    fResult = MAX_NUMBER(fNum_01,fNum_02);
    printf("\n\n");
    printf("Result Of Macro Function MAX_NUMBER() = %f \n",fResult);

    printf("\n\n");

    return 0;
}