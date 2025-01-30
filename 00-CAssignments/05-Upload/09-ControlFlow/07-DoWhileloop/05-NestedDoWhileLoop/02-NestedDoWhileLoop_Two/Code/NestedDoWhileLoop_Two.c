#include<stdio.h>
int main(void)
{
    //variable declaratioms
    int ganii_i,ganii_j,ganii_k;

    //code 
    printf("\n\n");

    ganii_i =1;
    do 
    {
        printf("i = %d\n",ganii_i);
        printf("-----------\n\n");

        ganii_j =1;
        do 
        {
            printf("\tj = %d\n",ganii_j);
            printf("\t------------\n\n");

            ganii_k = 1;
            do 
            {
                printf("\t\tk= %d\n",ganii_k);
                ganii_k++;
            }while(ganii_k <=3);
            printf("\n\n");
            ganii_j++;
        }while(ganii_j <=5);
        printf("\n\n");
        ganii_i++;
    }while(ganii_i <=10);
    return 0;
}