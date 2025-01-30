#include<stdio.h>//'stdio.h' conatins declarations of printf()

//entry point fuction =>main(=> Valid Return (int) and 3 paramters (int,argc,char* argv[],char *envp)

int main(int argc,char * argv[],char *envp[])
{
    //variable declarations
    int i;

    //code 
    printf("\n\n");
    printf("Hello World !!\n\n"); //Library fuction

    printf("Number of Command Line Arguments = %d \n\n",argc);

    printf("Command Line Arguments Passes To this Program Are :\n\n");
    for(i = 0; i <argc;i++)
    {
        printf("Command Line Argument Number %d = %s\n",(i +1),argv[i]);
    }
    printf("\n\n");

    printf("First 20 Environmental Variable Passed To This Program Are :\n'n");
    for( i= 0; i <  20; i++)
    {
        printf("Environmenatl Variable Number %d = %s\n",(i +1),envp[i]);
    }
    printf("\n\n");
    return 0;
}