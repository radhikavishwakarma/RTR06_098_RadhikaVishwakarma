#include<stdio.h>
int main(void)
{
   //varibale declarations
   int ganii_i,ganii_j,ganii_k;

   //code
   printf("\n\n");

   ganii_i = 1;
   while(ganii_i <=10)
   {
        printf("i = %d \n",ganii_i);
        printf("-----------\n\n");

        ganii_j = 1;
        while(ganii_j <= 5)
        {
            printf("\tj= %d \n",ganii_j);
            printf("\t------------\n\n");

            ganii_k =1;
            while(ganii_k <=3)
            {
                printf("\t\tk = %d\n",ganii_k);
                ganii_k++;
            }
            printf("\n\n");
            ganii_j++;
        }
        printf("\n\n");
        ganii_i++;
   }

    return 0;
}