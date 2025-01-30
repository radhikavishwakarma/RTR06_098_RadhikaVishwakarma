#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
int main(int argc, char *argv[],char *envp[])
int main(int argc, char *argv[],char *envp[])
int main(int argc, char *argv[],char *envp[])
int main(int argc,char *argv[],char *envp[])

{
    void MyAddition(int,int);

    int a,b;

    printf("Enter Intger Value for A :");
    scanf("%d",&a);

    printf("Enter Intger Value For B : ");
    scanf("%d",&b);

    MyAddition(a,b);

    return 0;
}

void MyAddition(int a,int b)
{
    int sum;

    sum = a + b;
    printf("sum of %d and %d  = %d \n\n",a,b,sum);
}