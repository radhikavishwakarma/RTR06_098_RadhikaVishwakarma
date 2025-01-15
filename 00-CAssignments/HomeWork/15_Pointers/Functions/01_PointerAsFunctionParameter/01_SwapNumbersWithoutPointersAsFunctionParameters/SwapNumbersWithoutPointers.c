#include <stdio.h>

int main(void)
{
	//Function Declaration
	void SwapNumbers(int, int);

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

	SwapNumbers(a, b); //***** Arguments Passed 'By Value' *****

	printf("\n\n");
	printf("********* After Swapping*********\n\n");
	printf("Value of 'A' =%d\n\n", a);
	printf("Value of 'A' =%d\n\n", b);

	return(0);
}

void SwapNumbers(int x, int y) // Value of 'a' is copied into 'x' and value 'b' is Copied into 'y'....Swapping
//Takes place Between 'x' and 'y', not between 'a' and 'b'.....
{
	//Variable Declarations
	int temp;

	//Code
	printf("\n\n");
	printf("***** Before Swapping *****\n\n");
	printf("Value of 'X' = %d\n\n", x);
	printf("Value of 'Y' = %d\n\n", y);

	temp = x;
	x = y;
	y = temp;

	printf("\n\n");
	printf("********* After Swapping*********\n\n");
	printf("Value of 'X' =%d\n\n", x);
	printf("Value of 'Y' =%d\n\n", y);

}
