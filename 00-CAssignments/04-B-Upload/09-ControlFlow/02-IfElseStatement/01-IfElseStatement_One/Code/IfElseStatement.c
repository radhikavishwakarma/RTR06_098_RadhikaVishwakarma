#include<stdio.h>
int main(void)
{
    //variable declarations 
    int a,b,p;

    //code
    a = 9;
    b = 30;
    p = 30;

    //Firsy of-else PAIR
    printf("\n\n");
    if(a <b)
    {
        printf("Entering FOrst if-blcok..\n\n");
        printf("A is LEss Than B !!!\n\n");
    }
    else
    {
        printf("Entering FOrst else-block...\n\n");
        printf("A Is NOT Less Than B !!!!\n\n");
    }
    printf("First if-else Pair Done !!!!\n");

    // ** SECOND if-else PAIR
    printf("\n\n");
    if( b!=p)
    {
        printf("ENtering Secod if-block..\n\nn");
        printf("B Is NOT Equal To P !!!\n\n");
    }
    else
    {
        printf("Entering Second else-block...\n\n");
        printf("B Is Equal To P !!!\n\n");
    }
    printf("Second if-else Pair Done !!!!\n\n");

    return 0;
}