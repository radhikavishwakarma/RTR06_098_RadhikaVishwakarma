#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    // variable declaration
    int i;
    int num;
    int sum = 0;

    // code
    if(argc == 1)
    {
        printf("\n\n");
        printf("No NUmbers given for addition !!! Exiting now...\n\n");
        printf("Usage : Commandline arguments Application <First Number> <Second Number>...\n\n");
        exit(0);
    }

    /* *** This program adds all commandline arguments given in INTEGER form only and outputs the SUM
    Due to use of atoi(), all commandline arguments of types other than 'int' are ignored ***
    */
    printf("\n\n");
    printf("Sum of all integer  commandline argument is: \n\n");
    for ( i = 1; i < argc; i++) // loop starts from i = 1 because, i = 0 will result in 'argv[i]' = 'argv[0]' whic is the name of the program itself i.e : CommandLineArgumentApplication.exe
    {
        num = atoi(argv[i]);
        sum = sum + num;
    }
    printf("Sum = %d\n\n", sum);

    return 0;
}
