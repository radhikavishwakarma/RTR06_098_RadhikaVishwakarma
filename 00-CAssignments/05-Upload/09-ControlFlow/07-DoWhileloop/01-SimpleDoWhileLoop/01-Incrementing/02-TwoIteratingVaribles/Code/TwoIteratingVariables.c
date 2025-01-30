#include<stdio.h>
int main(void)
{
    //variabale declarations
    int ganii_i,ganii_j;

    //code
    printf("\n\n");

    printf("PRinting Digits 1 to 10 and 10 to 100 \n\n");

    ganii_i = 1;
    ganii_j =10;
    do 
    {
        printf("\t %d \t %d\n",ganii_i,ganii_j);
        ganii_i++;
        ganii_j = ganii_j + 10;
    }while(ganii_i <=10 ,ganii_j <= 100);
    printf("\n\n");
    
    return 0;
}