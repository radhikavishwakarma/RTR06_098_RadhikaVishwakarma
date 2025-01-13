#include <stdio.h>
int main(void)
{
	//Function declarations
	void MathematicalOperations(int, int, int*, int*, int*, int*, int*);

	//Variable Declarations
	int a;
	int b;
	int answer_sum;
	int answer_difference;
	int answer_product;
	int answer_quotient;
	int answer_remainder;

	//Code
	printf("\n\n");
	printf("Enter Value of 'A' : ");
	scanf("%d", &a);

	printf("\n\n");
	printf("Enter Value of 'B' :");
	scanf("%d", &b);

	// Passing Address To Function.. Function Will Fill Them up With Values... Hence They Go Into The
	//The Function as addressed parameters and come out of the function filled with valid Values
	//Thus,(&answer_sum &answer_difference, &answer_product , &answer_quotient , & answer_remainder)
	//Are Called 'OUT PARAMETERS' OR 'PARAMETERIZED RETURN VAUES'..RETURN VALUES OF FUNCTION Coming VIA Parameters
	//Hence, Although Each Function Has Only One Return Value, Using The Concept Of Paramterized Return Values' our
	//Function 'MathematicalOperations()" Has Given us 5 Return Values !!!

 MathematicalOperations(a, b, &answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder);

	printf("\n\n");
	printf("********* Result ********* : \n\n");
	printf("Sum = %d\n\n", answer_sum);
	printf("Difference = %d\n\n", answer_difference);
	printf("Product = %d\n\n", answer_product);
	printf("Quotient = %d\n\n", answer_quotient);
	printf("Remainder = %d\n\n", answer_remainder);
	return(0);
}

void MathematicalOperations(int x, int y , int *sum , int *difference, int *product, int *quotient, int *remainder)
{
	//Code
	*sum = x + y; // Value at address 'sum' = (x + y)
	*difference = x - y; // Value at address 'difference' = (x - y)
	*product = x * y; // Value at address 'product' = (x * y)
	*quotient = x / y; // Value at address 'quotient' = (x / y)
	*remainder = x % y; // Value at address 'remainder' = (x % y)
}
