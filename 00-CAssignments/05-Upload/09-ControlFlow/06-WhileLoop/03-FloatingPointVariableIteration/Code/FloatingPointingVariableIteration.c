#include<stdio.h>
int main(void)
{
    //variable declarations
    float ganii_f;
    float ganii_f_num = 1.7f; //simply change this value ONLY to get different

    //code
    printf("\n\n");

    printf("Printing Numbers %f to %f : \n\n",ganii_f_num,(ganii_f_num * 10.f));

    ganii_f = ganii_f_num;
    while(ganii_f<=(ganii_f_num * 10.0f))
    {
        printf("\t %f\n",ganii_f);
        ganii_f = ganii_f + ganii_f_num;
    }
    printf("\n\n");

    return 0;
}