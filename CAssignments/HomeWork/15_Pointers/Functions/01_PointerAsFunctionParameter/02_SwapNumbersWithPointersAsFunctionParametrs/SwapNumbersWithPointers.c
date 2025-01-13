#include <stdio.h>

int main(void)
{
	//Function Declaration
	void SwapNumbers(int *, int *);

	//Variable Declaration
	int a;
	int b;

	//Code
	printf("\n\n");
	printf("Enter Value For 'A' :");
	scanf("%d", &a);

	printf("\n\n");
	printf("Enter Value 'B' :");
	scanf("%d", &b);

	printf("\n\n");
	printf("***** Before Swapping *****\n\n");
	printf("Value of 'A' = %d\n\n", a);
	printf("Value of 'B' = %d\n\n", b);

	SwapNumbers(&a, &b); //***** Arguments Passed 'By Reference / Address' *****

	printf("\n\n");
	printf("********* After Swapping*********\n\n");
	printf("Value of 'A' =%d\n\n", a);
	printf("Value of 'A' =%d\n\n", b);

	return(0);
}


// Address of 'a' is Copied Into 'x' address of 'b' is copied into 'y'.... so '&a' and 'x' are pointing to ONE
//and the SAME Address and '&b' and 'y' are pointing to one and the same address...
//Swapping takes place between 'value at address of x' (value at &a i.e : 'a') and 'value' at address of 'y' (value at &b i.e : 'b')..
//Hence,Swapping in this case takes place betwwen '*x' and '*y' As Well As between 'a' and 'b'...

void SwapNumbers(int *x, int *y) 
{
	//Variable Declarations
	int temp;

	//Code
	printf("\n\n");
	printf("***** Before Swapping *****\n\n");
	printf("Value of 'X' = %d\n\n", *x);
	printf("Value of 'Y' = %d\n\n", *y);

	temp = *x;
	*x = *y;
	*y = temp;

	printf("\n\n");
	printf("********* After Swapping*********\n\n");
	printf("Value of 'X' =%d\n\n", *x);
	printf("Value of 'Y' =%d\n\n", *y);

}

