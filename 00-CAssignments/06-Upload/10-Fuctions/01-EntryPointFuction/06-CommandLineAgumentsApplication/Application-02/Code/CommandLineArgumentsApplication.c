#include<stdio.h>// stdio.h contains declaration of 'printf()'
#include<stdlib.h>// 'stdlib'h' contains declaration of exit()

int main(int argc,char *argv[],char *envp[])
{
    //variable declaration
    int i;

    //code
    if(argc !=4)// Program name +  first name +middle name + surname = 4 commadline argumeny are required
    {
        printf("\n\n");
        printf("Invalid Usage !!! Existing Now ...\n\n");
        printf("Usage : CommandLineArgumentsApplication <first name> <middle name> <surname>\n\n");
        exit(0);
    }
    // This Program PRINTS YOPOUR FULL ANME AS ENTERED IN THE COMMAND LINE ARGUMENTS ***

    printf("\n\n");
    printf("Your Full Name Is : ");
    for(i = 1; i <argc; i++)// loop starts from i = 1 bcz i = 0 will result 
    //in 'argv[i] ='argv[0]' which is the name of the prigram itself i.e.CommandLine ArgumentApplication.exe
    {
        printf("%s",argv[i]);
    }
    printf("\n\n");

    return 0;
}