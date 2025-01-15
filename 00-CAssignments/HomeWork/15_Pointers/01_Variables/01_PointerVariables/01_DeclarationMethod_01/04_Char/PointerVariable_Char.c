#include<stdio.h>

int main(void)
{
	//Variable declarations
	char ch;
	char* ptr = NULL; //Declarations Method 1 :- *ptr' is a Variable of types 'Char'

	//Code
	ch = 'R';

	printf("\n\n");

	printf("********** Before ptr = &ch **********\n\n");
	printf("Value of 'ch'     =%c\n\n", ch);
	printf("Address Of 'ch'   =%p\n\n", &ch);
	printf("Value at Address of 'ch' = %c\n\n", *(&ch));

	//Assigning Address of variable 'ch' to pointer variable 'ptr'
	//'ptr' Now contain address of 'ch'... hence 'ptr' is SAME as &num
	ptr = &ch;

	printf("\n\n");

	printf("********** After ptr = &ch **********\n\n");
	printf("Value of 'ch'     =%cs\n\n", ch);
	printf("Address Of 'ch'   =%p\n\n", ptr);
	printf("Value at Address of 'ch' = %c\n\n", *ptr);

	return(0);
}


