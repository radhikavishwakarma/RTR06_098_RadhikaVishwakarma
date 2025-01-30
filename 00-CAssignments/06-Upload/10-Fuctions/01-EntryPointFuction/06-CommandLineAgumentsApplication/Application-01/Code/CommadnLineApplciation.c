#include<stdio.h>// '' stdio.hconatins declaration  or printf()
#include<ctype.h> //'ctype.h' conatins declaration or 'atoi()
#include<stdlib.h>// stdlib.h' conatins declaration of exit()

int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    int i;
    int num = 0;
    int sum =0;

    //code 
    if(argc == 1)
    {
        printf("\nn");
        printf("Np Number Given For Addition !!! Existing now ...\n\n");
        printf("Usage : CommandLine Arguments Application <first number >  <Second number>...\n\n");
        exit(0);

    }

    // ***THIS PROGRAMS ADDS ALL COMMAND LINE ARGUMENTS GIVEN IN INTEGER FORM ONLY AND OUTPUT THE SUM ***
    // ***DUR TO USE OF atoi(),ALL COMMAND LINE ARGUMENTS OF YTPES  OTHER THAN int  ARE IGNORED 

    printf("\n\n");
    printf("Sum of All Intger Command Line Argument Is :\nn");
    for(i = 0; i <argc ; i ++)
    {
        //loop starts from i =1  bcz i = 0; will result in 'argv[i] ='argv[0] which is the name of ther program itself i.e : Command Line ArgumentApplication.exe
        num = atoi(argv[i]);
        sum = sum + num;
    }

    printf("Sum = %d \n\n",sum);

    return 0;
}