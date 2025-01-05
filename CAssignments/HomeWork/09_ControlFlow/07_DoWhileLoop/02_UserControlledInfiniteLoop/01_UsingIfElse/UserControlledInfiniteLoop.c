#include<stdio.h>
#include<conio.h>

int main(void)
{
    // variable declaration
    char option, ch = '\0';

    // code
    printf("\n\n");
    printf("Once the infinite loop begins, enter 'Q' or 'q' to quit the infinite FOR loop\n\n");
    printf("Enter 'Y' or 'y' to initiate User COntrolled Infinite loop\n\n");

    option = getch();
    if (option == 'Y' || option == 'y')
    {
        do
        {
            printf("In loop...\n");
            ch = getch();
            if (ch == 'Q' || ch == 'q')
            {
                break;  // user controlled exiting from infinite loop
            }
        } while (1);  // infinite loop
        
        printf("\n\n");
        printf("Exiting user controlled infinite loop...");
        printf("\n\n");
    }
    else
    {
        printf("You must press 'Y' or 'y' to initiate the user controlled infinite loop... Please Try Again... \n\n");
    }

    return 0;
}