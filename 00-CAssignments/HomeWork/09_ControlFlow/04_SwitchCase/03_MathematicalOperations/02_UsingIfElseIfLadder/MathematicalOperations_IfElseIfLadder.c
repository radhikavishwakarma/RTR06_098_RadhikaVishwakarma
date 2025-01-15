#include<stdio.h>
#include<conio.h>

int main(void)
{
    // variable declaration
    int a, b;
    int result;
    char option, option_division;

    // code
    printf("\n\n");
    printf("Enter Value for 'A' : ");
    scanf("%d", &a);
    printf("Enter Value for 'B' : ");
    scanf("%d", &b);
    
    printf("Enter option in character : \n\n");
    printf("'A' or 'a' for Addition : \n");
    printf("'S' or 's' for Subtraction : \n");
    printf("'M' or 'm' for Multiplication : \n");
    printf("'D' or 'd' for Division : \n\n");

    printf("Enter option : ");
    option = getch();

    if (option == 'A' || option == 'a')
    {
        result = a + b;
        printf("Addition of A = %d and B = %d gives Result %d !!!\n\n", a, b, result);
    }
    else if (option == 'S' || option == 's')
    {
        if ( a >= b )
        {
            result = a - b;
            printf("Subtraction of B = %d from A = %d gives Result %d !!!\n\n", b, a, result);
        }
        else
        {
            result = b - a;
            printf("Subtraction of A = %d from B = %d gives Result %d !!!\n\n", a, b, result);
        }
    }
    else if (option == 'M' || option == 'm')
    {
        result = a * b;
        printf("Multiplication of A = %d and B = %d gives Result %d !!!\n\n", a, b, result);
    }
    else if (option == 'D' || option == 'd')
    {
        printf("Enter option in character : \n\n");
        printf("'Q' or 'q' or '/' for Quotient upon division : \n");
        printf("'R' or 'r' or '%%' for Remainder upon division : \n");
        printf("Enter option : ");
        option_division = getch();
        printf("\n\n");

        if (option_division == 'Q' || option_division == 'q' || option_division == '/')
        {
            if (a >= b)
            {
                result = a / b;
                printf("Division of A = %d by B = %d gives Quotient = %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b / a;
                printf("Division of B = %d by A = %d gives Quotient = %d !!!\n\n", b, a, result);
            }
        }
        else if (option_division == 'R' || option_division == 'r' || option_division == '%')
        {
            if (a >= b)
            {
                result = a / b;
                printf("Division of A = %d by B = %d gives Remainder = %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b / a;
                printf("Division of B = %d by A = %d gives Remainder = %d !!!\n\n", b, a, result);
            }
        }
        else
        {
            printf("Invalid character %c entered for division !!! Please try again...\n\n", option_division);
        }      
    }
    else
    {
        printf("Invalid character %c entered !!! Please try again...\n\n", option);
    }
    
    printf("If - Else If - Else Ladder Complete !!!\n\n");

    return 0;
}