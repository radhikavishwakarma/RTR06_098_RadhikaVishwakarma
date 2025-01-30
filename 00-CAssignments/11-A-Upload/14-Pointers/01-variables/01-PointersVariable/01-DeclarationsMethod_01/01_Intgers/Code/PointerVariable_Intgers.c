#include<stdio.h>

int main(void)
{
    //variable declarations
    int num ;
    int *ptr = NULL ;// declarations Method - *ptr is variable of type int

    // code
    num =10 ;

    printf("\n\n");
    printf("******BEFORE ptr = &num ********\n\n");
    printf("Value of num        %d\n\n",num);
    printf("Address oof num        %p\n\n",&num);
    printf("Value At Address of num     =%d\n\n",*(&num));
    // assigning address of variable num to pointer variable ptr
    // ptr now conatins address of num.hence ptr is same as &num
    ptr = &num;

    printf("\n\n");
    printf("*********After ptr = &num **********\n\n");
    printf("Value of num        = %d\n\n",num);
    printf("Address of num      = %p\n\n",ptr);
    printf("Value At Address of num = %d\n\n",*ptr);

    return 0;
}