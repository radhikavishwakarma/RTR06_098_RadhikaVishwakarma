#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

//entry -Point Fuction =?main()=> Valid ReturnType (int) and Parametrs (int argc,char *argv[],char *envp[])
int main(int argc,char *argv[],char *envp[])
{
    //fuction prototype/  declaration /signature
    int MyAddition(int,int);

    //variable declaration : local variable to main()

    int a,b,result ;

    //code 
    printf("\n\n");
    printf("Enter Intger Value For 'A' :");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Intger Value For 'B ' :");
    scanf("%d",&b);

    result = MyAddition(a,b);//fuction call
    printf("\n\n");
    printf("Sum of %d And %d = %d \n\n",a,b,result);

    return 0;
}

// *************USER DEFINED FUCTION : METHOD OF DEFINITION 4 **********
// *********VALID (int) RETURN VALUE, VALID PARAMETERS (int,int)***********

int MyAddition(int a, int b)//fuction defition 
{
    //varibale declaration s : local variablles to MyAddition()
    int sum ;

    //code
    sum = a + b ;
    return sum ;
}