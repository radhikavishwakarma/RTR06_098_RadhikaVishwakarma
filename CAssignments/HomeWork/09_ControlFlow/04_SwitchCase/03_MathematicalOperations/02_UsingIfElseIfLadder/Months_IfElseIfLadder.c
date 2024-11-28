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

    if ( num_month == 1) // like 'case 1'
        printf("Month number %d is January!!!\n\n", num_month);
    else if (num_month == 2) // like 'case 2'
        printf("Month number %d is February!!!\n\n", num_month);
    else if ( num_month == 3) // like 'else if'
        printf("Month number %d is March!!!\n\n", num_month);
    else if ( num_month == 4) // like 'else if'
        printf("Month number %d is April!!!\n\n", num_month);
    else if ( num_month == 5) // like 'else if'
        printf("Month number %d is May!!!\n\n", num_month);
    else if ( num_month == 6) // like 'else if'
        printf("Month number %d is June!!!\n\n", num_month);
    else if ( num_month == 7) // like 'else if'
        printf("Month number %d is July!!!\n\n", num_month);
    else if ( num_month == 8) // like 'else if'
        printf("Month number %d is August!!!\n\n", num_month);
    else if ( num_month == 9) // like 'else if'
        printf("Month number %d is September!!!\n\n", num_month);
    else if ( num_month == 10) // like 'else if'
        printf("Month number %d is October!!!\n\n", num_month);
    else if ( num_month == 11) // like 'else if'
        printf("Month number %d is November!!!\n\n", num_month);
    else if ( num_month == 12) // like 'else if'
        printf("Month number %d is December!!!\n\n", num_month);
    else // like 'default'... just like 'default' is optional in switch case, so is 'else' in if-else if-else ladder...
        printf("Invalid month number %d entered !!! Please try again...\n\n", num_month);

    printf("If - Else If - Else Ladder Complete!!!\n");

    return 0;
}