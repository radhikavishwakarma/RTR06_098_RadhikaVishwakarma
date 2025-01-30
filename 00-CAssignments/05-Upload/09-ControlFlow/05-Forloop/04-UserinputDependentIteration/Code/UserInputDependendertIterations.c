#include<stdio.h>
int main(void)
{
    //variable declarations
    int i_num ,num ,i;

    //code
    printf("\n\n");

    printf("ENter An Intger Value From which Iteration Must Begin : ");
    scanf("%d",&i_num);

    printf("How many Digits Do You want to print from %d Onward ? : ",i_num);
    scanf("%d",&num);

    printf("Printing Digits %d to %d : \n\n ",i_num,(i_num + num));

    for(i = i_num ; i<= (i_num +num) ; i ++)
    {
        printf("\t %d\n",i);
    }
    printf("\n\n");

    return 0;
}
