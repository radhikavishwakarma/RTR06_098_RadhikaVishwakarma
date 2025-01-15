#include <stdio.h>

int main(void)
{
	//Variable Declarations
	int num;
	int* ptr = NULL;
	int* copy_ptr = NULL;

	// Code
	num = 5;
	ptr = &num;

	printf("\n\n");
	printf("******* BEFORE copy_ptr = ptr *********\n\n");
	printf(" num =  %d\n", num);
	printf(" &num =  %p\n", &num);
	printf(" *(&num) =  %d\n", *(&num));
	printf(" ptr =  %p\n", ptr);
	printf(" *ptr =  %d\n", *ptr);

	//'ptr' is an integer pointer variable....that it can hold the address of any integer variable only
	//'copy_ptr' is another integer pointer variable
	// if ptr = &num...'ptr' will contain of integer variable 'num'
	//if 'ptr' is assigned to 'copy_ptr' , 'copy_ptr' will also contain address of integer variable 'num'
	//Hence,now both 'ptr' and 'copy_ptr' will point to 'num'.....

	copy_ptr = ptr; // copy_ptr = ptr = &num

	printf("\n\n");
	printf("******* BAFTER copy_ptr = ptr *********\n\n");
	printf(" num =      %d\n", num);
	printf(" &num =     %p\n", &num);
	printf(" *(&num) =  %d\n", *(&num));
	printf(" ptr =      %p\n", ptr);
	printf(" *ptr =     %d\n", *ptr);
	printf(" copy_ptr = %p\n", copy_ptr);
	printf(" copy_ptr = %d\n", *copy_ptr);

	return(0);

}
