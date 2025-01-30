#include<stdio.h>

int main(void)
{
    //variable declarations
    float num;
    float *ptr = NULL;//Declaratons method 1 - *ptr s variable of type float

    // code
    num = 6.9f;

    printf("\n\n");

    printf("*******88BEFORE ptr = &num*********\n\n");
    printf("Value of num        %f\n\n",num);
    printf("Address of num      = %p\n\n",&num);
    printf("Value At Address if num = %f \n\n",*(&num));

    //Assigning address  of variable num to pointer variable ptr
    //..ptr now contains address of num ..hence ptr is SAme as &numptr = &num ;
    ptr = &num;

    printf("\n\n");
    printf("*********After ptr = &num *******\n\n");
    printf("Value of num        = %f\n\n",num);
    printf("Address of num      = %p\n\n",ptr);
    printf("Value At Address of num = %f\n\n",*ptr);

    return 0;

}