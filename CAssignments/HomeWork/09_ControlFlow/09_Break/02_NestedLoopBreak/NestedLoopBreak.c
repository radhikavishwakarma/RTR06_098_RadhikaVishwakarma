#include<stdio.h>
#include<conio.h>

int main(void)
{
    // variable declaration
    int i, j;
    char ch;

    // code
    printf("\n\n");

    for (i = 1; i <= 20; i++)
    {
        for ( j = 0; j <= 20; j++)
        {
            if ( j > i)
            {
                break;
            }
            else
            {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
    
    return 0;
}