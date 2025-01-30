#include<stdio.h>
#include<conio.h>
int main(void)
{
    //variable declarations
    int ganii_i,ganii_j;

    //code
    printf("\n\n");
    for(ganii_i =1; ganii_i<= 20; ganii_i++)
    {
        for(ganii_j =1; ganii_j<=20; ganii_j++)
        {
            if(ganii_j>ganii_i)
            {
                break;
            }
            else 
            {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}