#include<stdio.h>
int main(void)
{
    //variable declartions
    int i,j,k;

    //code
    printf("\n\n");
    for(i =1; i<= 10;i++)
    {
        printf("i = %d \n",i);
        printf("---------------\n\n");
        for(j =1;j<= 5;j++)
        {
            printf("\t j = %d\n",j);
            printf("\t-----------\n");
            for(k=1;k<= 3;k++)
            {
                printf("\t\t = %d\n",k);
            }
            printf("\n\n");
        }
        printf("\n\n");
    }
    return 0;
}