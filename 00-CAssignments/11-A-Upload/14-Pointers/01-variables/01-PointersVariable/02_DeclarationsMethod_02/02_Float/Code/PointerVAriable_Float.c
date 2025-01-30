#include<stdio.h>

int main(void)
{
    //variable declartions
    float ganii_num;
    float* ganii_ptr = NULL;// declarations ethod 2 = ptr is a variable of type float*

    //code
    ganii_num = 6.9f;

    printf("*****Before ptr = &num*****\n\n");
    printf("Value Of num        =%f\n\n",ganii_num);
    printf("Address of num      =%p\n\n",&ganii_num);
    printf("Vaue At Address of num = %f \n\n",*(&ganii_num));

    //Assigning Address of avriable num to pointer variable ptr
    //ptr new coantins address num...hence ptr is same s &num
    ganii_ptr = &ganii_num;

    printf("\n\n");
    printf("****After ptr = &num*****\n\n");
    printf("Value of num = %f\n\n",ganii_num);
    printf("Address of num      = %p\n\n",ganii_ptr);
    printf("Value At Address of num  = %f\n\n",*ganii_ptr);

    return 0;
}