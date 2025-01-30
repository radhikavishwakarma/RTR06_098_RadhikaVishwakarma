#include<stdio.h>
int main(void)
{
    //variable declarations
    char ganii_option ,ganii_ch = '\0';

    //code
    printf("\n\n");
    printf("Once The Infinite Loop Beins,Enter 'Q' or 'q' To Quit The Infinite For Loop : \n\n");
    printf("Enter 'Y' or 'y' To Initiate User Cotrolled Infinit Loop :");
    printf("\n\n");
    ganii_option = getch();
    if(ganii_option == 'Y' || ganii_option =='y')
    {
        while(1) //infinit loop
        {
            printf("In Loop...\n");
            ganii_ch = getch();
            if(ganii_ch == 'Q' || ganii_ch == 'q')
                break;//User Controlled Existing From Infinit Loop
        }
        printf("\n\n");
        printf("EXISTING USER COTROLLED INFINIT LOOP...");
        printf("\n\n");
    }
    else 
        printf("You Must Press 'Y' or 'y' To Initiate The User Cotrolled Infinite Loop..Please Try Again...\n\n");

    return 0;

}