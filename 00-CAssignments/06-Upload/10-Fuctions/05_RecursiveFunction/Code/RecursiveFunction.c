//
//RecursiveFunction.c
//
//
//Created by ASTROMEDICOMP on 31/05/20
///

#include<stdio.h>

int main(int agrc,char *argv[],char *envp[])
{
    //variable declarations
    unsigned int num;

    //function prototype
    void recursive(unsigned int);

    //code
    printf("\n\n");
    printf("ENter Any Number  : \n\n");
    scanf("%u",&num);

    printf("\n\n");
    printf("Output Of Recursive Function : \n\n");

    recursive(num);

    printf("\n\n");

}

void recursive(unsigned int n)
{
    //code
    printf("n = %d \n",n);

    if(n > 0)
    {
        recursive(n -1);
    }
}