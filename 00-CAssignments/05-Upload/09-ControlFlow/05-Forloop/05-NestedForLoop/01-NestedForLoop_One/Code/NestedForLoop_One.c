#include<stdio.h>
int main(void)
{
    //varible declarations
    int i,j;

    //code
    printf("\n\n");
    for(i = 1; i <=10; i++)
    {
        printf("i = %d\n",i);
        printf("--------------\n");
        for(j = 0; j<=5;j++)
        {
            printf("\t j = %d\n",j);
        }
        printf("\n\n");
    }
    return 0;
}