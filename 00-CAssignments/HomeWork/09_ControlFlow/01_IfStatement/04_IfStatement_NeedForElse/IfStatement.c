#include<stdio.h>

int main(void)
{
    // variable declaration
    int age;

    // code
    printf("\n\n----------------------------------\n");
    printf("Enter age: ");
    scanf("%d", &age);

    if (age >= 18)
    {
        printf("You are ELIGIBLE for voting !!!\n\n", age);
    }
    printf("You are NOT ELIGIBLE for voting !!!\n\n", age);
    
    return(0);
}