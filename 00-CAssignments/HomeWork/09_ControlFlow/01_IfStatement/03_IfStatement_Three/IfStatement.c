#include<stdio.h>

int main(void)
{
    // variable declaration
    int num;

    // code
    printf("\n\n----------------------------------\n");
    printf("Enter a number: ");
    scanf("%d", &num);

    if (num < 0)
    {
        printf("Num = %d is LESS THAN 0 (Negative).\n\n", num);
    }

    if ((num > 0) && (num <= 100))
    {
        printf("Num = %d is BETWEEN 0 and 100.\n\n", num);
    }

    if ((num > 100) && (num <= 200))
    {
        printf("Num = %d is BETWEEN 100 and 200.\n\n", num);
    }

    if ((num > 200) && (num <= 300))
    {
        printf("Num = %d is BETWEEN 200 and 300.\n\n", num);
    }

    if ((num > 300) && (num <= 400))
    {
        printf("Num = %d is BETWEEN 300 and 400.\n\n", num);
    }

    if ((num > 400) && (num <= 500))
    {
        printf("Num = %d is BETWEEN 400 and 500.\n\n", num);
    }

    if (num > 500)
    {
        printf("Num = %d is GREATER THAN 500.\n\n", num);
    }


    return(0);
}