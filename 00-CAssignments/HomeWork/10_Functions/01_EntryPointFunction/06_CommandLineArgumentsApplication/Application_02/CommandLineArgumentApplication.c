#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    // variable declaration
    int i;

    // code
    if(argc != 4)  // Program name  + first name + middle name + surname = 4 command line arguments are required
    {
        printf("\n\n");
        printf("Invalid Usage !!! Exiting now...\n\n");
        printf("Usage : Commandline arguments Application <First Name> <Middle Name> <Surname>...\n\n");
        exit(0);
    }

    /* *** This program prints your full name as entered in the commandline arguments
    */
    printf("\n\n");
    printf("Your Full name is: \n\n");
    for ( i = 1; i < argc; i++) // loop starts from i = 1 because, i = 0 will result in 'argv[i]' = 'argv[0]' whic is the name of the program itself i.e : CommandLineArgumentApplication.exe
    {
        printf("%s", argv[i]);
    }
    printf("\n\n");

    return 0;
}
