#include<stdio.h>

int main(void)
{
	//Variable declarations
	int num;
	int* ptr = NULL; //Declarations Method 2 :- *ptr' is a Variable of types 'int*'

	//Code
	num = 16;

	printf("\n\n");

	printf("********** Before ptr = &num **********\n\n");
	printf("Value of 'num'     =%d\n\n", num);
	printf("Address Of 'num'   =%p\n\n", &num);
	printf("Value at Address of 'num' = %d\n\n", *(&num));

	//Assigning Address of variable 'num' to pointer variable 'ptr'
	//'ptr' Now contain address of 'num'... Hence 'ptr' is SAME as &num
	ptr = &num;

	printf("\n\n");

	printf("********** After ptr = &num **********\n\n");
	printf("Value of 'num'     =%d\n\n", num);
	printf("Address Of 'num'   =%p\n\n", ptr);
	printf("Value at Address of 'num' = %d\n\n", *ptr);

	return(0);
}
