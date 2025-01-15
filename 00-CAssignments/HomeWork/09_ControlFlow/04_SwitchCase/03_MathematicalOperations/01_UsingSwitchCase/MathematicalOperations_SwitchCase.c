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

    switch (option)
    {
    case 'A': // FALL THROUGH CONDITION FOR 'A' and 'a'
    case 'a':
        result = a + b;
        printf("Addition of A = %d and B = %d gives Result %d !!!\n\n", a, b, result);
        break;
    case 'S': // FALL THROUGH CONDITION FOR 'S' and 's'
    case 's':
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
        break;
    case 'M': // FALL THROUGH CONDITION FOR 'M' and 'm'
    case 'm':
        result = a * b;
        printf("Multiplication of A = %d and B = %d gives Result %d !!!\n\n", a, b, result);
        break;
    case 'D': // FALL THROUGH CONDITION FOR 'D' and 'd'
    case 'd':
        printf("Enter option in character : \n\n");
        printf("'Q' or 'q' or '/' for Quotient upon division : \n");
        printf("'R' or 'r' or '%%' for Remainder upon division : \n");
        printf("Enter option : ");
        option_division = getch();
        printf("\n\n");

        switch (option_division)
        {
        case 'Q':// FALL THROUGH CONDITION FOR 'Q' and 'q' and '/'
        case 'q':
        case '/':
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
            break; // 'break' of case 'Q' or 'q' or '/'

        case 'R':// FALL THROUGH CONDITION FOR 'R' and 'r' and '%'
        case 'r':
        case '%':
            if (a >= b)
            {
                result = a % b;
                printf("Division of A = %d by B = %d gives Remainder = %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b % a;
                printf("Division of B = %d by A = %d gives Remainder = %d !!!\n\n", b, a, result);
            }
            break; // 'break' of case 'R' or 'r' or '%'
        
        default:  // 'default case for switch(option_division)'
            printf("Invalid character %c entered for division !!! Please try again...\n\n", option_division);
            break; // 'break' of 'default' of switch(option_division)
        }   // ending curly brace of switch(option_division)

        break;  // 'break' of case 'D' or 'd'

    default: // 'default case for switch(option)'
        printf("Invalid character %c entered !!! Please try again...\n\n", option);
        break;
    }   // ending curly brace of switch(option)

    printf("Switch Case Block Complete !!!\n\n");

    return 0;
}