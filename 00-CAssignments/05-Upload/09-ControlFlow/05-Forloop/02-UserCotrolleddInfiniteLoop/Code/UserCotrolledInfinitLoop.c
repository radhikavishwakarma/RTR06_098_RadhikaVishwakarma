#include<stdio.h>
int main(void)
{
    //variable declarations
    char option , ch ='\0';

    //code
    printf("\n\n");
    printf("Once the Infinite Loop Begis,ENter 'Q' or 'q' to Quit The Infinite For Loop :\n\n");
    printf("Enter 'Y' or 'y' To Inifinite User Cotrolled Infinite Loop :");
    printf("\n\n");
    option = getch();
    if(option == 'Y' || option == 'y')
    {
        for(;;) //inifite Loop
        {
        printf("In Loop..\n");
        ch= getch();
        if(ch == 'Q' || ch == 'q')
            break;//user Corolled Exitting From Infinite Loop
        }
    }
    printf("\n\n");
    printf("Exiting User Cotrolled Infinit Loop...");
    printf("\n\n");
    return  0;
}
