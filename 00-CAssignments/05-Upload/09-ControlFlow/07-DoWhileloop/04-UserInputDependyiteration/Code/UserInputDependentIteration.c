#include<stdio.h>
int main(void)
{
    //variable declarations
    int ganii_i_num,ganii_num,ganii_i;

    //code
    printf("\n\n");

    printf("ENter An Intger Value from Which Iteration Must Begin : ");
    scanf("%d",&ganii_i_num);

    printf("How Many Digits Do you Want To print From %d onwards ?: ",ganii_i_num);
    scanf("%d",&ganii_num);

    printf("Printing DIgits %d to %d : \n\n",ganii_i_num,(ganii_i_num +ganii_num));

    ganii_i = ganii_i_num;
    do
    {
        printf("\t%d\n",ganii_i);
        ganii_i++;
    }while(ganii_i <=(ganii_i_num +ganii_num));
    printf("\n\n");

    return 0;
}