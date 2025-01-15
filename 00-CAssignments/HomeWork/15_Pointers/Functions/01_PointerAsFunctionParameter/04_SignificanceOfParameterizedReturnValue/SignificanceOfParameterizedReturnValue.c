#include<stdio.h>

enum
{
	NEGATIVE = -1,
	ZERO,
	POSITIVE
};

int main(void)
{
	//Function Declarations
	int Difference(int, int, int*);

	//Variable Declaration
	int a;
	int b;
	int answer, ret;

	//Code
	printf("\n\n");
	printf("Enter Value of 'A' : ");
	scanf("%d", &a);

	//Code
	printf("\n\n");
	printf("Enter Value of 'B' : ");
	scanf("%d", &b);

	ret = Difference(a, b, &answer);

	printf("\n\n");
	printf("Difference of %d And %d = %d\n\n", a, b, answer);

	if (ret = POSITIVE)
		printf("The Difference Of %d and %d Is Positive !!!\n\n", a, b);

	else if (ret == NEGATIVE)
		printf("The Difference of %d And %d Is Negative !!!\n\n", a, b);

	else
		printf("The Difference Of %d And %d Is Zero !!!!\n\n", a, b);

	return(0);
}

//We want our function difference() to perform 2 JOBS...
//One, Is to subtract The Input numbers ('y' form 'x') And the second is to tell Whether the Difference of 'x' and 'y'
// Is Positive Or Negative or Zero...
//But Any Function has only one valid return value the How Can we Manage to Return Two Values to The Calling Funtion
//This Is Where Paramater Return Value Comes Into This Picture..
//We Can Return The Actual Difference oF 'x' and 'y' That is The Actual Answer Value via Out Parameter /Parameter Return Value
//And  we can return The Status of The Answer (POSITIVE / NEGATIVE / ZERO)VIA the acutal return value of the function..

int Difference(int x, int y, int *diff)
{
	//Code 
	*diff = x - y;

	if (*diff > 0)
		return (POSITIVE);

	else if (*diff < 0)
		return (NEGATIVE);

	else
		return(ZERO);
}
