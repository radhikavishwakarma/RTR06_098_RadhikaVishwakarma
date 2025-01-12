#include<stdio.h>

int main(void)
{
	//Variable declarations
	double num;
	double *ptr = NULL; //Declarations Method 1 :- *ptr' is a Variable of types 'Double'

	//Code
	num = 2.34343434f;

	printf("\n\n");

	printf("********** Before ptr = &num **********\n\n");
	printf("Value of 'num'     =%lf\n\n", num);
	printf("Address Of 'num'   =%p\n\n", &num);
	printf("Value at Address of 'num' = %lf\n\n", *(&num));

	//Assigning Address of variable 'num' to pointer variable 'ptr'
	//'ptr' Now contain address of 'num'... hence 'ptr' is SAME as &num
	ptr = &num;

	printf("\n\n");

	printf("********** After ptr = &num **********\n\n");
	printf("Value of 'num'     =%lf\n\n", num);
	printf("Address Of 'num'   =%p\n\n", ptr);
	printf("Value at Address of 'num' = %lf\n\n", *ptr);

	return(0);
}


