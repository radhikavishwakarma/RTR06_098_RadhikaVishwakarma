#include<stdio.h>
int main(void)
{
    //variable declarations
    int age;

    //code
    printf("Enter Age : ");
    scanf("%d",&age);
    if(age >= 18)
    {
        printf("You Are Eligible For Voting !!!!!\n\n");
    }
    printf("You Are Not Eligible For Voting !!!!\n\n");
    return 0;

}