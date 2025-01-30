#include<stdio.h>// 'stdio.h conatins devalration of print()

//Entry-Point Fuction =?main()=>Valid Return Type (int) and 3 Parameters (int argc, char*argv[],char *envp[])
int main(int argc,char *argv[],char *envp[])
{
    //fuction prototype/ declaration / signature
    int MyAddition(void);

    //variable delaration : local variable to main()
    int result ;

    // code
    result = MyAddition();

    printf("\n\n");
    printf("sum = %d \n\n",result);
    return 0;
}

// **USER DEFINED FUCTION : METHOD OF DEFIENITION 2***********
// **** VALID (int) RETURN VALUE ,NO PARAMATER *****

int MyAddition(void)//fuction defienition
{
    //variable declaration :ocal variable to MyAddition()
    int a,b, sum ;

    //code
    printf("\n\n");
    printf("Enter Intger Value For 'A' : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Intger Value For 'B' :");
    scanf("%d",&b);

    sum = a + b;

    return sum ;
}