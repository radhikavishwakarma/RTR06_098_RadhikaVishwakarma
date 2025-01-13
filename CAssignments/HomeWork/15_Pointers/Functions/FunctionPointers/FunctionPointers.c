#include<stdio.h>

int main(void)
{
	//Function Declarations
	int AddIntegers(int, int);
	int SubtractIntegers(int, int);
	float AddFloats(float, float);

	//Variable Declarations
	typedef int (*AddIntsFnPtr)(int, int);
	AddIntsFnPtr ptrAddTwoIntegers = NULL;
	AddIntsFnPtr ptrFunc = NULL;

	typedef float (*AddFloatsFnPtr)(float, float);
	AddFloatsFnPtr ptrAddTwoFloats = NULL;

	int iAnswer = 0;
	float fAnswer = 0.0f;

	//Code
	ptrAddTwoIntegers = AddIntegers;
	iAnswer = ptrAddTwoIntegers(9, 30);
	printf("\n\n");
	printf("Sum Of Integers = %d\n\n", iAnswer);

	ptrFunc = SubtractIntegers;
	iAnswer = ptrFunc(9, 30);
	printf("\n\n");
	printf("Subtraction Of Integers = %d\n\n", iAnswer);

	ptrAddTwoFloats = AddFloats;
	fAnswer = ptrAddTwoFloats(11.45f, 8.2f);
	printf("\n\n");
	printf("Sum of Floating-point Numbers = %f\n\n", fAnswer);

	return(0);
}

int AddIntegers(int a, int b)
{
	//Variable Declarations
	int c;

	//Code
	c = a + b;
	return(c);

}

int SubtractIntegers(int a, int b)
{
	//Variable Declarations
	int c;

	//Code
	if (a > b)
		c = a - b;

	else
		c = b - a;

	return(c);
	
}

float AddFloats(float f_num1, float f_num2)
{
	//Variable Declarations
	float ans;

	//Code
	ans = f_num1 + f_num2;
	return(ans);
}
