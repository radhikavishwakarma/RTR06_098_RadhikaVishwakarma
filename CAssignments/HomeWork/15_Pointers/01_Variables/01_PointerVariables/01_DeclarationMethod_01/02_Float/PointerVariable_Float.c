#include<stdio.h>

int main(void)
{
	//Variable declarations
	float num;
	float *ptr = NULL; //Declarations Method 1 :- *ptr' is a Variable of types 'float'

	//Code
	num = 6.9f;

	printf("\n\n");

	printf("********** Before ptr = &num **********\n\n");
	printf("Value of 'num'     =%f\n\n", num);
	printf("Address Of 'num'   =%p\n\n", &num);
	printf("Value at Address of 'num' = %f\n\n", *(&num));

	//Assigning Address of variable 'num' to pointer variable 'ptr'
	//'ptr' Now contain address of 'num'... hence 'ptr' is SAME as &num
	ptr = &num;

	printf("\n\n");

	printf("********** After ptr = &num **********\n\n");
	printf("Value of 'num'     =%f\n\n", num);
	printf("Address Of 'num'   =%p\n\n", ptr);
	printf("Value at Address of 'num' = %f\n\n", *ptr);

	return(0);
}

