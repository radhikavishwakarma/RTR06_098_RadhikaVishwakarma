#include<stdio.h>// 'stdio.h' conatins declaration of 'printf()'

//Entr-Poiny Fuction =>main=>Valid Return Type (int )and 3 Parameters(int argc,char *argv[],char *envp[])

int main(int agrc,char *argv[],char *envp[])
{
    //fuction prototype / declaration / signature 
    void MyAddition(int,int);

    //variable declaration : local variable to main()
    int a,b;

    //code
    printf("\n\n");
    printf("Enter Intger Value For 'A' : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("ENter Intger Value For 'B ' :");
    scanf("%d",&b);

    MyAddition(a,b);// fuction  call

    return 0;
}

//**USER DEFINED FUCTION : METHOD OF DEFIENITION 3 */
// ****NP RETURN VALUE ,VALID PARAMATERS (int,int)

void MyAddition(int a,int b)// fuction defienition
{
    //variable declarations : local variable to MyAddition()
    int sum ;

    //code
    sum = a + b;
    printf("\n\n");
    printf("Sum of %d And %d = %d \n\n",a,b,sum);
}