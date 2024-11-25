#include<stdio.h>

int main(void)
{
    // variable declaration
    int age;

    // code
    printf("\n\n----------------------------------\n");
    printf("Enter Age: ");
    scanf("%d", &age);

    if (age >= 18)
    {
        printf("You are eligible for voting!!!\n\n");
    }

    return(0);
}