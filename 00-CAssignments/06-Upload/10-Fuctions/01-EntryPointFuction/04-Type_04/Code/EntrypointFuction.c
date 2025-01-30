#include<stdio.h> // stdio.h conatins declarationd of printf()

//Entry-point = main() =>valid Return type (int) and 2 paramters (int argc,char *argv[])

int main(int argc,char *argv[])
{
    //variable declarations
    int i;

    //code
    printf("\n\n");
    printf("Hello World !!!\n\n"); //libreary fuction
    printf("Number of Command Line Arguments = %d\n\n",argc);

    printf("Command Linbe arguments passed to this program are : \n'n");
    for(i =0 ; i <argc; i++)
    {
        printf("Command Line Argumenst %d = %s \n",(i+1),argv[i]);
    }
    printf("\n\n");

    return 0;
}