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

    do
    {
        do
        {
            printf("\n");
            printf("In loop...\n");
            ch = getch();  // control flow waits for character input...
        } while (ch == 'Q' || ch == 'q');
        printf("\n\n");
        printf("Exiting user controlled infinite loop...");
        printf("\n\n");

        printf("Do you want to begin user controlled Infinite loop again?... (Y/y - Yes, any other key - No) :");
        option = getch();
    } while (option == 'Y' || option == 'y');

    return 0;
}