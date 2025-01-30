#include<stdio.h>
int main(void)
{
    //variable declaration 
    int ganii_i_num,ganii_num ,ganii_i;

    //code
    printf("\n\n");

    printf("ENter An Intger Value From which Iteration Must Begin : ");
    scanf("%d",&ganii_i_num);

    printf("How many Digits Do you want to Print From %d onwards ? :",ganii_i_num);
    scanf("%d",&ganii_num);

    printf("Printing Digits %d to %d : \n\n",ganii_i_num,(ganii_num *ganii_i_num));

    ganii_i = ganii_i_num;
    while(ganii_i <= (ganii_i_num + ganii_num))
    {
        printf("\t %d \n",ganii_i);
        ganii_i++;
    }
    printf("\n\n");

    return 0;
}