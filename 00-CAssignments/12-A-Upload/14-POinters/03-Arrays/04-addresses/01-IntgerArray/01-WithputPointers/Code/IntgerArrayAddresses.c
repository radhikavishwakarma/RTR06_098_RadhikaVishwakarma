#include<stdio.h>

int main(void)
{
    //variablde declarations
    int ganii_iArray[10];
    int ganii_i;

    for(ganii_i = 0; ganii_i < 10;ganii_i++)
        ganii_iArray[ganii_i] = (ganii_i + 1) * 3;
    printf("\n\n");
    printf("Elements of the Intger Array  :\n\n");
    for(ganii_i = 0; ganii_i < 10; ganii_i++)
        printf("iArray[%d] : = %d\n",ganii_i,ganii_iArray[ganii_i]);

    printf("\n\n");
    printf("ELements of the Intger Array  : \n\n");
    for(ganii_i = 0; ganii_i <10; ganii_i++)
        printf("'iArray[%d] = %d \t \t  Address = %p\n'",ganii_i,ganii_iArray[ganii_i],&ganii_iArray[ganii_i]);

    return 0;

}