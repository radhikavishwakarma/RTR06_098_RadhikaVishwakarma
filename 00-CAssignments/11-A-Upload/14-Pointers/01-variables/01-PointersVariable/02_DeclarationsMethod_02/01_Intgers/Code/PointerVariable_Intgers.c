#include<stdio.h>

int main(void)
{
    //variblde declarations
    int ganii_num;
    int* ganii_ptr = NULL;//declarations method 2 - ptr is a variable of type if

    //code
    ganii_num = 10;

    printf("\n\n");
    printf("***********Before ptr= &num*****88\n\n");
    printf("Value of num        = %d\n\n",ganii_num);
    printf("Addres sof num =    %p\n\n",&ganii_num);
    printf("Value AtAddress of num = %d\n\n",*(&ganii_num));

    //Assigning addres of variable num to pointer variable potr
    // ptr now contains address of num ..hnece ptr is same as &num
    ganii_ptr = &ganii_num;

    printf("\n\n");
    printf("Value Of num        =%d\n\n",ganii_num);
    printf("Address of num      ==%p\n\n",ganii_ptr);
    printf("Value At Address of num     = %d\n\n",*ganii_ptr);

    return 0;

}