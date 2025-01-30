#include<stdio.h>
int main(void)
{
    //variable declarations
    float ganii_f;
    float ganii_f_num = 1.7f;//simply chage this value only get diffrent ouputs..

    //code 
    printf("\n\n");

    printf("Printing Number %f to %f : \n\n",ganii_f_num,(ganii_f_num * 10.0f));

    ganii_f = ganii_f_num ;
    do 
    {
        printf("\t%f\n",ganii_f);
        ganii_f = ganii_f + ganii_f_num;
    }while(ganii_f <=(ganii_f_num * 10.0f));
    printf("\n\n");

    return 0;
}