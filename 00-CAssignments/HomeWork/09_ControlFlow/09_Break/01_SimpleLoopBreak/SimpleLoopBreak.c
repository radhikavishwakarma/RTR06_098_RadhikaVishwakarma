#include<stdio.h>
#include<conio.h>

int main(void)
{
    // variable declaration
    int i;
    char ch;

    // code
    printf("\n\n");
    printf("Printing Even Numbers from 0 to 100 For every user input. Exiting the loop when user enters character 'Q' or 'q' : \n\n");
    printf("Enter character 'Q' or 'q' to Exit Loop : \n\n");

    for (i = 1; i <= 100; i++)
    {
        printf("\t%d\n", i);
        ch = getch();
        if (ch == 'Q' || ch == 'q')
        {
            break;
        }
    }

    printf("\n\n");
    printf("Exiting Loop...\n");
    printf("\n\n");
    
    return 0;
}