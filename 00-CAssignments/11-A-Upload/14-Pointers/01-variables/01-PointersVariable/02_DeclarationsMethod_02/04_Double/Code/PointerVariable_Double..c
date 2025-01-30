#include<stdio.h>

int main(void)
{
    //variable declarations
    double ganii_num;
    double* gnaii_ptr = NULL;// Declaratiosn Method2- ptr is a variable of type double*

    //code
    ganii_num = 2.4343434f;

    printf("\n\n");
    printf("*****Before ptr = &num****\n\n");
    printf("Value of num   =%lf\n\n",ganii_num);
    printf("Address of num = %p\n\n",&ganii_num);
    printf("Value At Address of num = %lf\n\n",*(&ganii_num));

    //Asssigning address of variable num to pointer variable ptr
    //ptr not conatins address of num..hence ptr is same as &num
    gnaii_ptr = &ganii_num;

    printf("\n\n");

    printf("****After ptr = &num****\n\n");
    printf("Value of num    = %lf\n\n",ganii_num);
    printf("Address of num  =%p\n\n",gnaii_ptr);
    printf("Value At address of num  = %lf\n\n",*gnaii_ptr);

    return 0;    
}

