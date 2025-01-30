#include<stdio.h>//'stdio.h' contains declaration of 'printf()

// Entry-Point Fuction =>main =>valid Return Type (int) and 3 Parameters (int argc,char *argv[],char *envp[])
int main(int argc, char *argv[],char *envp[])
{
    // fuction prototype /declaration //signature
    void MyAddition(void);

    //code
    MyAddition(); // fuction call
    return 0;
}

//**USER DEFIENED FUCTION : METHOD OF DEFINITION  1*/
// ***No RETURN VAUE ,NO PARAMATERS ////

void MyAddition(void)// Fuction defination
{
    //variable declaration : local variable to MyAddition()
    int a,b, sum;

    //code
    printf("\n\n");
    printf("ENter Value For 'A' : ");
    scanf("%d",&a);

    printf("Enter Intger Value For 'B' ");
    scanf("%d",&b);

    sum = a + b;
    printf("\n\n");
    printf("Sum of %d And %d = %d \n\n",a,b,sum);

    
}