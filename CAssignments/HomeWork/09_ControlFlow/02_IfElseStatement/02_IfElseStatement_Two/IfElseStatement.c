#include<stdio.h>

int main(void)
{
    // variable declaration
    int age;

    // code
    printf("\n\n");
    printf("Enter age: ");
    scanf("%d", &age);
    printf("\n\n");

    if (age >= 18)
    {
        printf("Entering First if-block...\n\n");
        printf("You are eligible for voting !!!\n\n");
    }
    else
    {
        printf("Entering First else-block...\n\n");
        printf("You are NOT eligible for voting !!!\n\n");
    }
    printf("Bye !!!\n\n");

    return 0;
}