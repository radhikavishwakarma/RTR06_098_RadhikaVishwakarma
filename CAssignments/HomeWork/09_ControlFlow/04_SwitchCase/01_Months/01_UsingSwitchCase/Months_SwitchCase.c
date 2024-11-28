#include<stdio.h>

int main(void)
{
    // variable declaration
    int num_month;

    // code
    printf("\n\n");
    printf("Enter number of Month (1 to 12): ");
    scanf("%d", &num_month);
    printf("\n\n");

    switch (num_month)
    {
    case 1: // like 'if'
        printf("Month number %d is January!!!\n\n", num_month);
        break;
    case 2: // like 'else if'
        printf("Month number %d is February!!!\n\n", num_month);
        break;
    case 3: // like 'else if'
        printf("Month number %d is March!!!\n\n", num_month);
        break;
    case 4: // like 'else if'
        printf("Month number %d is April!!!\n\n", num_month);
        break;
    case 5: // like 'else if'
        printf("Month number %d is May!!!\n\n", num_month);
        break;
    case 6: // like 'else if'
        printf("Month number %d is June!!!\n\n", num_month);
        break;
    case 7: // like 'else if'
        printf("Month number %d is July!!!\n\n", num_month);
        break;
    case 8: // like 'else if'
        printf("Month number %d is August!!!\n\n", num_month);
        break;
    case 9: // like 'else if'
        printf("Month number %d is September!!!\n\n", num_month);
        break;
    case 10: // like 'else if'
        printf("Month number %d is October!!!\n\n", num_month);
        break;
    case 11: // like 'else if'
        printf("Month number %d is November!!!\n\n", num_month);
        break;
    case 12: // like 'else if'
        printf("Month number %d is December!!!\n\n", num_month);
        break;
    
    default: // like ending OPTIONAL 'else'... just like terminating 'else' is optional in if-else if-else ladder, so is the 'default' case optional in switch case
        printf("Invalid month number %d entered !!! Please try again...\n\n", num_month);
        break;
    }

    printf("Switch Case Block Complete !!!\n\n");

    return 0;
}