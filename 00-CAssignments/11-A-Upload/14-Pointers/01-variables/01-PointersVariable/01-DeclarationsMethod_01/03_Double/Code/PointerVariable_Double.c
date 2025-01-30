#include<stdio.h>

int main(void)
{
    //variable declarations
    double num;
    double *ptr = NULL; //Declarations Method1 *ptr is a variable of type double

    //code  
    num = 2.343435435f;

    printf("\n\n");

    printf("*********BEFORE ptr = &num**********\n\n");
    printf("Value of num        %lf\n\n",num);
    printf("Address of num      = %p\n\n",&num);
    printf("Value At Address of num = %lf\n\n",*(&num));

    //Assigning address of variable num to pointer variable ptr
    //ptr now contains addres of num...hence ptr is same as &num
    ptr = &num;

    printf("\n\n");
    printf("******FTER ptr = &num*****\n\n");
    printf("Value of num = %lf\n\n",ptr);
    printf("Address of num =    %p\n\n",ptr);
    printf("Value At Address of num = %lf\n\n",*ptr);

    return 0;
}